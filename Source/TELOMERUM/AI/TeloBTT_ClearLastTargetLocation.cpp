// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloBTT_ClearLastTargetLocation.h"
#include "BehaviorTree/BlackboardComponent.h"

UTeloBTT_ClearLastTargetLocation::UTeloBTT_ClearLastTargetLocation()
{
	NodeName = "Clear LastTargetLocation";
}

EBTNodeResult::Type UTeloBTT_ClearLastTargetLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return EBTNodeResult::Failed;

	BB->ClearValue(TEXT("LastTargetLocation"));

	return EBTNodeResult::Succeeded;
}
