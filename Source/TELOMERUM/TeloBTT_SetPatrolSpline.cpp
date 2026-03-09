// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloBTT_SetPatrolSpline.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TeloEnemyCharacter.h"
#include "TeloPatrolSpline.h"

UTeloBTT_SetPatrolSpline::UTeloBTT_SetPatrolSpline()
{
	NodeName = TEXT("Set PatrolSpline");
}

EBTNodeResult::Type UTeloBTT_SetPatrolSpline::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AI = OwnerComp.GetAIOwner();
	if (!AI) return EBTNodeResult::Failed;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return EBTNodeResult::Failed;

	ATeloEnemyCharacter* EnemyCharacter = Cast<ATeloEnemyCharacter>(AI->GetPawn());
	if (!EnemyCharacter) return EBTNodeResult::Failed;

	ATeloPatrolSpline* PatrolSpline = EnemyCharacter->GetPatrolSpline();
	if (!PatrolSpline) return EBTNodeResult::Failed;

	BB ->SetValueAsObject(TEXT("PatrolSpline"), PatrolSpline);

	return EBTNodeResult::Succeeded;
}
