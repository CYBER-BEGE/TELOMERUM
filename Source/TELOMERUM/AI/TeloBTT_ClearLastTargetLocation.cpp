// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloBTT_ClearLastTargetLocation.h"

#include "BehaviorTree/BlackboardComponent.h"

UTeloBTT_ClearLastTargetLocation::UTeloBTT_ClearLastTargetLocation()
{
	NodeName = "Clear LastTargetLocation";
}

EBTNodeResult::Type UTeloBTT_ClearLastTargetLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory)
{
	UBlackboardComponent* BlackBoard = OwnerComponent.GetBlackboardComponent();
	if (!BlackBoard) return EBTNodeResult::Failed;

	// LastTargetLocation 블랙보드 키 값 초기화
	BlackBoard->ClearValue(TEXT("LastTargetLocation"));

	return EBTNodeResult::Succeeded;
}
