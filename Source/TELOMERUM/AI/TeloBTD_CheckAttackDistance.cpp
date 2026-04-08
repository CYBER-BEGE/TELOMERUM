// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloBTD_CheckAttackDistance.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "TeloCharacterBase.h"

/* 공격 범위 내에 적이 있으면 True, 없으면 False 리턴 */
bool UTeloBTD_CheckAttackDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory) const
{
	AAIController* AIController = OwnerComponent.GetAIOwner();
	if (!AIController) return false;

	APawn* Owner = AIController->GetPawn();
	if (!Owner) return false;

	ATeloCharacterBase* OwnerCharacter = Cast<ATeloCharacterBase>(Owner);
	if (!OwnerCharacter) return false;

	UBlackboardComponent* BlackBoard = OwnerComponent.GetBlackboardComponent();
	if (!BlackBoard) return false;

	AActor* TargetActor = Cast<AActor>(BlackBoard->GetValueAsObject(TEXT("TargetActor")));
	if (!TargetActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] CheckAttackDistance: TargetActor 없음"), *Owner->GetActorLabel());
		return false;
	}

	FVector TraceA;
	FVector TraceB;

	if (!OwnerCharacter->GetAttackTracePoint(TraceA, TraceB))
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] CheckAttackDistance: AttackTracePoint 없음"), *Owner->GetActorLabel());
		return false;
	}
	
	const FVector TargetLocation = TargetActor->GetActorLocation();

	const float DistanceA = FVector::Dist(TraceA, TargetLocation);
	const float DistanceB = FVector::Dist(TraceB, TargetLocation);
	const float ClosestDistance = FMath::Min(DistanceA, DistanceB);

	const float SocketDistance = FVector::Dist(TraceA, TraceB);

	UE_LOG(LogTemp, Warning, TEXT("[%s] CheckAttackDistance: %s"), *Owner->GetActorLabel(), (ClosestDistance <= SocketDistance + OwnerCharacter->GetAttackRadius()) ? TEXT("TRUE") : TEXT("FALSE"));
	return ClosestDistance <= (SocketDistance + OwnerCharacter->GetAttackRadius());
}
