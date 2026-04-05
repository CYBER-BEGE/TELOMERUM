// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloBTT_AIInvestigate.h"

#include "AIController.h"
#include "NavigationSystem.h"

UTeloBTT_AIInvestigate::UTeloBTT_AIInvestigate()
{
	NodeName = TEXT("AI Investigate");
}

EBTNodeResult::Type UTeloBTT_AIInvestigate::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComponent.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	UBlackboardComponent* BlackBoard = OwnerComponent.GetBlackboardComponent();
	if (!BlackBoard) return EBTNodeResult::Failed;
	
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(AIController);
	if (!NavSystem) return EBTNodeResult::Failed;

	FNavLocation RandomLocation;
	FVector Center = BlackBoard->GetValueAsVector(TEXT("LastTargetLocation")); // Investigate 중심점

	// 반경 내에서 도달할 수 있는 랜덤 위치 지정
	bool bFoundReachableLocation = NavSystem->GetRandomReachablePointInRadius(Center, InvestigateRadius, RandomLocation);
	if (!bFoundReachableLocation) return EBTNodeResult::Failed;

	// InvestigateLocation 블랙보드 키에 랜덤 위치 저장
	BlackBoard->SetValueAsVector(TEXT("InvestigateLocation"), RandomLocation.Location);

	return EBTNodeResult::Succeeded;
}
