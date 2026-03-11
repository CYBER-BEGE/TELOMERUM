// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ATeloAIController::ATeloAIController()
{
	PrimaryActorTick.bCanEverTick = true; // Tick 활성화

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SetPerceptionComponent(*AIPerceptionComponent);

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	SightConfig->SightRadius = 1500.0f;
	SightConfig->LoseSightRadius = 1800.0f;
	SightConfig->PeripheralVisionAngleDegrees = 75.0f;
	SightConfig->SetMaxAge(5.0f);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ATeloAIController::OnTargetperceived);

	BehaviorTree = nullptr;
	BlackboardComponent = nullptr;
}

void ATeloAIController::BeginPlay()
{
	Super::BeginPlay();
	
	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);

		UBlackboardComponent* BB = GetBlackboardComponent();
		if (BB && GetPawn())
		{
			BB->SetValueAsVector(TEXT("SpawnLocation"), GetPawn()->GetActorLocation());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ATeloAIController] BehaviorTree is NULL")); // 컴포넌트 값 확인
	}
}
void ATeloAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//DrawSightDebug();
}

/*
void ATeloAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UE_LOG(LogTemp, Warning, TEXT("AI Possessed"));

	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);

		UBlackboardComponent* BB = GetBlackboardComponent();
		if (BB)
		{
			BB->SetValueAsVector(TEXT("SpawnLocation"), InPawn->GetActorLocation());
		}
	}
}*/

void ATeloAIController::DrawSightDebug()
{
    APawn* InPawn = GetPawn();
    if (!InPawn || !SightConfig) return;

    UBlackboardComponent* BB = GetBlackboardComponent();
    if (!BB) return;

    // 현재 Focus Actor 확인
    AActor* FocusActor = Cast<AActor>(BB->GetValueAsObject(TEXT("TargetActor")));
    
    FVector Location = InPawn->GetActorLocation();
    FVector Forward;

    if (FocusActor)
    {
        // Focus Actor 위치를 향하도록 Forward 벡터 계산
        Forward = (FocusActor->GetActorLocation() - Location).GetSafeNormal();
    }
    else
    {
        // Focus Actor가 없으면 Pawn의 Forward 사용
        Forward = InPawn->GetActorForwardVector();
    }

    float SightRadius = SightConfig->SightRadius;
    float HalfAngleDeg = SightConfig->PeripheralVisionAngleDegrees / 2;

    // 시야 원
    DrawDebugCircle(GetWorld(), Location, SightRadius, 32, FColor::Red, false, 0.1f, 0, 2, FVector(1,0,0), FVector(0,1,0), false);

    // 좌우 경계선 계산 (Focus 방향 기준)
    FVector LeftDir = Forward.RotateAngleAxis(-HalfAngleDeg, FVector::UpVector);
    FVector RightDir = Forward.RotateAngleAxis(HalfAngleDeg, FVector::UpVector);

    // 좌/우 경계선 Draw
    DrawDebugLine(GetWorld(), Location, Location + LeftDir * SightRadius, FColor::Green, false, 0.1f, 0, 2);
    DrawDebugLine(GetWorld(), Location, Location + RightDir * SightRadius, FColor::Green, false, 0.1f, 0, 2);

    // 선택적으로 Forward 방향 라인 (Focus 기준)
    DrawDebugLine(GetWorld(), Location, Location + Forward * SightRadius, FColor::Blue, false, 0.1f, 0, 2);
}

void ATeloAIController::OnTargetperceived(AActor* Actor, FAIStimulus Stimulus)
{
	UBlackboardComponent* BB = GetBlackboardComponent();
	if (!BB || !Actor) return;

	if (Stimulus.WasSuccessfullySensed())
	{
		BB->SetValueAsObject(TEXT("TargetActor"), Actor);
		SetFocus(Actor);

		UE_LOG(LogTemp, Warning, TEXT("[%s] %s 가 보임"), *GetNameSafe(GetPawn()), *GetNameSafe(Actor));
	}
	else
	{
		BB->ClearValue(TEXT("TargetActor"));
		ClearFocus(EAIFocusPriority::Gameplay);
		
		BB->SetValueAsVector(TEXT("LastTargetLocation"), Stimulus.StimulusLocation);
		DrawDebugSphere(GetWorld(), Stimulus.StimulusLocation, 50.0f, 12, FColor::Yellow, false, 2.0f);

		UE_LOG(LogTemp, Warning, TEXT("[%s] %s 를 놓침"), *GetNameSafe(GetPawn()), *GetNameSafe(Actor));
	}
}