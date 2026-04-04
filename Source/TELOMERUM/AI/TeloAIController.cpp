// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloAIController.h"

#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "NavigationSystem.h"

ATeloAIController::ATeloAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	// Perception Component 설정
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SetPerceptionComponent(*AIPerceptionComponent);

	// Perception - Sight 설정
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1500.0f;
	SightConfig->LoseSightRadius = 1800.0f;
	SightConfig->PeripheralVisionAngleDegrees = 75.0f;
	SightConfig->SetMaxAge(5.0f);

	// Sight 감지 대상 설정 - 추후 변경필요
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	// Perception - Damage 설정
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));
	DamageConfig->SetMaxAge(10.0f);

	// Perception - Hearing 설정
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->HearingRange = 800.0f;
	HearingConfig->SetMaxAge(8.0f);

	// Hearing 감지 대상 설정 - 추후 변경필요
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;

	// Perception Component에 Sight 적용
	AIPerceptionComponent->ConfigureSense(*SightConfig);

	// Perception Component에 Damage 적용
	AIPerceptionComponent->ConfigureSense(*DamageConfig);
	AIPerceptionComponent->SetDominantSense(DamageConfig->GetSenseImplementation());

	// Perception Component에 Hearing 적용
	AIPerceptionComponent->ConfigureSense(*HearingConfig);

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
			BlackBoard->SetValueAsBool(TEXT("bDamaged"), false);
		}
	}
	else UE_LOG(LogTemp, Error, TEXT("ATeloAIController: BehaviorTree is NULL")); // 블루프린트의 BT 등록 확인
}

/* ==================== Perception ==================== */

void ATeloAIController::OnTargetperceived(AActor* Actor, FAIStimulus Stimulus)
{
	UBlackboardComponent* BlackBoard = GetBlackboardComponent();
	if (!BlackBoard || !Actor) return;

	const FAISenseID SenseID = Stimulus.Type;
	FString SenseTypeName = TEXT("Unknown");

	/* 시야 감지 */
	if (SenseID == UAISense::GetSenseID(UAISense_Sight::StaticClass()))
	{
		SenseTypeName = "Sight";
	}

	/* 데미지 감지 */
	if (SenseID == UAISense::GetSenseID(UAISense_Damage::StaticClass()))
	{
		SenseTypeName = "Damage";
	}

	/* 청각 감지 */
	if (SenseID == UAISense::GetSenseID(UAISense_Hearing::StaticClass()))
	{
		SenseTypeName = "Hearing";
	}

	UE_LOG(LogTemp, Display, TEXT("[%s] AIController Perception %s / 타겟: %s / 감지 타입: %s"),
		*GetPawn()->GetActorNameOrLabel(),
		Stimulus.WasSuccessfullySensed() ? TEXT("성공") : TEXT("실패"),
		*Actor->GetActorNameOrLabel(),
		*SenseTypeName);

	UpdateTargetActor(Actor, Stimulus.StimulusLocation);
}

void ATeloAIController::UpdateTargetActor(AActor* Actor, const FVector& LastStimulusLocation)
{
	UBlackboardComponent* BlackBoard = GetBlackboardComponent();
	if (!BlackBoard || !Actor) return;

	const FAISenseID SightID = UAISense::GetSenseID(UAISense_Sight::StaticClass());
	const FAISenseID DamageID = UAISense::GetSenseID(UAISense_Damage::StaticClass());
	const FAISenseID HearingID = UAISense::GetSenseID(UAISense_Hearing::StaticClass());

	const bool bSeen = IsSensing(Actor, SightID);
	const bool bDamaged = IsSensing(Actor, DamageID);
	const bool bHeard = IsSensing(Actor, HearingID);

	if(bSeen || bDamaged || bHeard)
	{
		BlackBoard->SetValueAsObject(TEXT("TargetActor"), Actor);
		//SetFocus(Actor);
		return;
	}
	
	BlackBoard->ClearValue(TEXT("TargetActor"));
	//ClearFocus(EAIFocusPriority::Gameplay);

	// LastTargetLocation 보정
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	FNavLocation ProjectedLocation;

	if (NavSystem && NavSystem->ProjectPointToNavigation(LastStimulusLocation, ProjectedLocation, ProjectionExtent))
	{
		BlackBoard->SetValueAsVector(TEXT("LastTargetLocation"), ProjectedLocation.Location);
	}
	else
	{
		BlackBoard->ClearValue(TEXT("LastTargetLocation"));
		UE_LOG(LogTemp, Warning, TEXT("[%s] LastTargetLocation 보정 실패"), *GetPawn()->GetActorNameOrLabel());
	}
}

bool ATeloAIController::IsSensing(AActor* Actor, FAISenseID SenseID) const
{
	if (!AIPerceptionComponent || !Actor) return false;

	FActorPerceptionBlueprintInfo PerceptionInfo;
	AIPerceptionComponent->GetActorsPerception(Actor, PerceptionInfo);

	for (const FAIStimulus& Stimulus : PerceptionInfo.LastSensedStimuli)
	{
		if (Stimulus.Type == SenseID && Stimulus.WasSuccessfullySensed()) return true;
	}

	return false;
}