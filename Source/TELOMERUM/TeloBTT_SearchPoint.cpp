// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloBTT_SearchPoint.h"
#include "AIController.h"
#include "NavigationSystem.h"

UTeloBTT_SearchPoint::UTeloBTT_SearchPoint()
{
	NodeName = TEXT("Search Point");
}

EBTNodeResult::Type UTeloBTT_SearchPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AI = OwnerComp.GetAIOwner();
	if (!AI) return EBTNodeResult::Failed;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return EBTNodeResult::Failed;

	FVector Center = BB->GetValueAsVector(TEXT("LastTargetLocation"));
	
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(AI);
	if (!NavSys) return EBTNodeResult::Failed;

	FNavLocation RandomPoint;

	bool bFound = NavSys->GetRandomReachablePointInRadius(Center, SearchRadius,	RandomPoint);
	if (!bFound) return EBTNodeResult::Failed;

	BB->SetValueAsVector(TEXT("SearchLocation"), RandomPoint.Location);

	return EBTNodeResult::Succeeded;
}
