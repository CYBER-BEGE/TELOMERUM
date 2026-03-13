// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "NavigationSystem.h"

ATeloAIController::ATeloAIController()
{
	PrimaryActorTick.bCanEverTick = false;

	// Perception Component 설정
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SetPerceptionComponent(*AIPerceptionComponent);

	// Perception - Sight 설정
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1500.0f;
	SightConfig->LoseSightRadius = 1800.0f;
	SightConfig->PeripheralVisionAngleDegrees = 75.0f;
	SightConfig->SetMaxAge(5.0f);

	// Sight 감지 대상 설정
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	// Perception Component에 Sight 적용
	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ATeloAIController::OnTargetperceived);

	ProjectionExtent = FVector(200.0f, 200.0f, 300.0f); // LastTargetLocation 보정 범위 지정

	// BT, BB 초기화
	BehaviorTree = nullptr;
	BlackboardComponent = nullptr;
}

void ATeloAIController::BeginPlay()
{
	Super::BeginPlay();
	
	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);

		// SpawnLocation 저장
		UBlackboardComponent* BlackBoard = GetBlackboardComponent();
		if (BlackBoard && GetPawn())
		{
			BlackBoard->SetValueAsVector(TEXT("SpawnLocation"), GetPawn()->GetActorLocation());
		}
	}
	else UE_LOG(LogTemp, Error, TEXT("ATeloAIController: BehaviorTree is NULL")); // 블루프린트의 BT 등록 확인
}

void ATeloAIController::OnTargetperceived(AActor* Actor, FAIStimulus Stimulus)
{
	UBlackboardComponent* BlackBoard = GetBlackboardComponent();
	if (!BlackBoard || !Actor) return;

	if (Stimulus.WasSuccessfullySensed()) // 타겟 감지 성공
	{
		// Set Target
		BlackBoard->SetValueAsObject(TEXT("TargetActor"), Actor);
		SetFocus(Actor);

		UE_LOG(LogTemp, Warning, TEXT("[%s] %s 가 보임"), *GetPawn()->GetActorNameOrLabel(), *Actor->GetActorNameOrLabel());
	}
	else // 타겟 놓침(감지 실패)
	{
		// Clear Target
		BlackBoard->ClearValue(TEXT("TargetActor"));
		ClearFocus(EAIFocusPriority::Gameplay);
		
		// LastTargetLocation 보정
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		FNavLocation ProjectedLocation;

		if (NavSystem && NavSystem->ProjectPointToNavigation(Stimulus.StimulusLocation, ProjectedLocation, ProjectionExtent))
		{
			BlackBoard->SetValueAsVector(TEXT("LastTargetLocation"), ProjectedLocation.Location); // 보정 성공, LastTargetLocation 저장
		}
		else 
		{
			BlackBoard->ClearValue(TEXT("LastTargetLocation"));
			UE_LOG(LogTemp, Warning, TEXT("LastTargetLocation 보정 실패")); // 보정 실패, LastTargetLocation 초기화
		}
		
		UE_LOG(LogTemp, Warning, TEXT("[%s] %s 를 놓침"), *GetNameSafe(GetPawn()), *GetNameSafe(Actor));
	}
}