// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloEnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ATeloEnemyAIController::ATeloEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true; // Tick 활성화

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SetPerceptionComponent(*AIPerceptionComponent);

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	SightConfig->SightRadius = 1500.0f;
	SightConfig->LoseSightRadius = 1800.0f;
	SightConfig->PeripheralVisionAngleDegrees = 120.0f;
	SightConfig->SetMaxAge(5.0f);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ATeloEnemyAIController::OnTargetperceived);

	BehaviorTree = nullptr;
	BlackboardComponent = nullptr;
}

void ATeloEnemyAIController::BeginPlay()
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
}
void ATeloEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawSightDebug();
}

/*
void ATeloEnemyAIController::OnPossess(APawn* InPawn)
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

void ATeloEnemyAIController::DrawSightDebug()
{
	APawn* InPawn = GetPawn();
	if (!InPawn || !SightConfig) return;

	FVector Location = InPawn->GetActorLocation();
	float SightRadius = SightConfig->SightRadius;
	float AngleRad = FMath::DegreesToRadians(SightConfig->PeripheralVisionAngleDegrees / 2);
	FVector Forward = InPawn->GetActorForwardVector();

	// 시야 범위 시각화
	DrawDebugSphere(GetWorld(), Location, SightRadius, 32, FColor::Red, false, 0.1f, 0, 2);
	DrawDebugCone(GetWorld(), Location, Forward, SightRadius, AngleRad, AngleRad, 32, FColor::Green, false, 0.1f, 0, 1);
}

void ATeloEnemyAIController::OnTargetperceived(AActor* Actor, FAIStimulus Stimulus)
{
	UBlackboardComponent* BB = GetBlackboardComponent();
	if (!BB || !Actor) return;

	if (Stimulus.WasSuccessfullySensed())
	{
		BB->SetValueAsObject(TEXT("TargetActor"), Actor);
		BB->SetValueAsVector(TEXT("LastKnownTargetLocation"), Actor->GetActorLocation());
		UE_LOG(LogTemp, Warning, TEXT("%s 감지됨"), *GetNameSafe(Actor));
	}
	else
	{
		BB->ClearValue(TEXT("TargetActor"));
		UE_LOG(LogTemp, Warning, TEXT("%s 놓침"), *GetNameSafe(Actor));
	}
}