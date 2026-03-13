// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloBTD_CheckAttackDistance.h"
#include "AIController.h"
#include "TeloCharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"

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

	float AttackDistance = OwnerCharacter->GetAttackDistance(); // 실제 공격 범위
	float Distance = FVector::Dist(Owner->GetActorLocation(), TargetActor->GetActorLocation()); // 공격 대상과의 거리

	UE_LOG(LogTemp, Warning, TEXT("[%s] CheckAttackDistance: %s"), *Owner->GetActorLabel(), (Distance <= AttackDistance) ? TEXT("TRUE") : TEXT("FALSE"));
	return Distance <= AttackDistance;
}
