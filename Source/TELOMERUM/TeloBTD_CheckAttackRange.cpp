// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloBTD_CheckAttackRange.h"
#include "AIController.h"
#include "TeloEnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UTeloBTD_CheckAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory) const
{
	// AIController 가져오기
	AAIController* AIController = OwnerComponent.GetAIOwner();
	if (!AIController) return false;

	// Pawn 가져오기
	APawn* Pawn = AIController->GetPawn();
	if (!Pawn) return false;

	// EnemyCharacter로 캐스트
	ATeloEnemyCharacter* Enemy = Cast<ATeloEnemyCharacter>(Pawn);
	if (!Enemy) return false;

	// Blackboard에서 TargetActor 가져오기
	UBlackboardComponent* BB = OwnerComponent.GetBlackboardComponent();
	if (!BB) return false;

	AActor* Target = Cast<AActor>(BB->GetValueAsObject(TEXT("TargetActor")));
	if (!Target) return false;

	float AttackRange = Enemy->GetCalculatedAttackDistance(); // 이미 계산된 실제 공격 범위
	float Distance = FVector::Dist(Pawn->GetActorLocation(), Target->GetActorLocation());

	return Distance <= AttackRange;
}
