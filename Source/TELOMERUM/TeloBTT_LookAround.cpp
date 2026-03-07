// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloBTT_LookAround.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"

UTeloBTT_LookAround::UTeloBTT_LookAround()
{
	NodeName = "Look Around";
}

EBTNodeResult::Type UTeloBTT_LookAround::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AI = OwnerComp.GetAIOwner();
	if (!AI) return EBTNodeResult::Failed;

	FRotator Rot = AI->GetControlRotation();
	Rot.Yaw += YawOffset;

	AI->SetControlRotation(Rot);

	return EBTNodeResult::Succeeded;
}
