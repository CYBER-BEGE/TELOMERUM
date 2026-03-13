// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloBTT_SetPatrolSpline.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TeloEnemyCharacter.h"
#include "TeloPatrolSpline.h"

UTeloBTT_SetPatrolSpline::UTeloBTT_SetPatrolSpline()
{
	NodeName = TEXT("Set PatrolSpline Key");
}

EBTNodeResult::Type UTeloBTT_SetPatrolSpline::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComponent.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	UBlackboardComponent* BlackBoard = OwnerComponent.GetBlackboardComponent();
	if (!BlackBoard) return EBTNodeResult::Failed;

	ATeloEnemyCharacter* EnemyCharacter = Cast<ATeloEnemyCharacter>(AIController->GetPawn());
	if (!EnemyCharacter) return EBTNodeResult::Failed;

	ATeloPatrolSpline* PatrolSpline = EnemyCharacter->GetPatrolSpline();
	if (!PatrolSpline) return EBTNodeResult::Failed;

	// PatrolSpline 블랙보드에 저장
	BlackBoard->SetValueAsObject(TEXT("PatrolSpline"), PatrolSpline);

	return EBTNodeResult::Succeeded;
}
