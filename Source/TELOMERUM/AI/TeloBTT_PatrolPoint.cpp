// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloBTT_PatrolPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TeloPatrolSpline.h"

UTeloBTT_PatrolPoint::UTeloBTT_PatrolPoint()
{
	NodeName = TEXT("Patrol Point");
}

EBTNodeResult::Type UTeloBTT_PatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB) return EBTNodeResult::Failed;

	ATeloPatrolSpline* PatrolSpline = Cast<ATeloPatrolSpline>(BB->GetValueAsObject(TEXT("PatrolSpline")));
	if (!PatrolSpline) return EBTNodeResult::Failed;

	const int32 TotalPoints = PatrolSpline->GetTotalPoints();
	if (TotalPoints <= 0) return EBTNodeResult::Failed;

	const int32 CurrentIndex = BB->GetValueAsInt(TEXT("PatrolIndex"));
	int32 NextIndex = INDEX_NONE;

	if (bUsePingPong)
	{
		bool bDirection = BB->GetValueAsBool(TEXT("PatrolDirection"));
		NextIndex = PatrolSpline->GetNextIndexPingPong(CurrentIndex, bDirection);
		BB->SetValueAsBool(TEXT("PatrolDirection"), bDirection);
	}
	else
	{
		NextIndex = PatrolSpline->GetNextIndexLoop(CurrentIndex);
	}
	
	if (NextIndex == INDEX_NONE) return EBTNodeResult::Failed;
	
	const FVector NextLocation = PatrolSpline->GetPatrolPointLocation(NextIndex);

	BB->SetValueAsInt(TEXT("PatrolIndex"), NextIndex);
	BB->SetValueAsVector(TEXT("PatrolLocation"), NextLocation);

	return EBTNodeResult::Succeeded;
}
