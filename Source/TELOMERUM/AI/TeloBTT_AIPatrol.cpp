// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloBTT_AIPatrol.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TeloPatrolSpline.h"

UTeloBTT_AIPatrol::UTeloBTT_AIPatrol()
{
	NodeName = TEXT("AI Patrol");
}

EBTNodeResult::Type UTeloBTT_AIPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory)
{
	UBlackboardComponent* BlackBoard = OwnerComponent.GetBlackboardComponent();
	if (!BlackBoard) return EBTNodeResult::Failed;

	// 순찰 경로 정보 가져오기
	ATeloPatrolSpline* PatrolSpline = Cast<ATeloPatrolSpline>(BlackBoard->GetValueAsObject(TEXT("PatrolSpline")));
	if (!PatrolSpline) 
	{
		UE_LOG(LogTemp, Error, TEXT("AIPatrol: 순찰 경로 없음"));
		return EBTNodeResult::Failed;
	}

	const int32 TotalPoints = PatrolSpline->GetTotalNodes(); // 총 노드 개수
	if (TotalPoints <= 0) return EBTNodeResult::Failed;

	const int32 CurrentIndex = BlackBoard->GetValueAsInt(TEXT("PatrolIndex")); // 현재 노드 인덱스
	int32 NextIndex = INDEX_NONE; // 다음 노드 인덱스 초기화

	if (bUsePingPong) // 왕복 순찰
	{
		bool bDirection = BlackBoard->GetValueAsBool(TEXT("PatrolDirection"));
		NextIndex = PatrolSpline->GetNextIndexPingPong(CurrentIndex, bDirection);
		BlackBoard->SetValueAsBool(TEXT("PatrolDirection"), bDirection);
	}
	else // 순환 순찰
	{
		NextIndex = PatrolSpline->GetNextIndexLoop(CurrentIndex);
	}
	
	if (NextIndex == INDEX_NONE) return EBTNodeResult::Failed;
	
	const FVector NextLocation = PatrolSpline->GetPatrolNodeLocation(NextIndex); // 다음 노드의 위치

	// 순찰 정보 블랙보드에 저장
	BlackBoard->SetValueAsInt(TEXT("PatrolIndex"), NextIndex);
	BlackBoard->SetValueAsVector(TEXT("PatrolLocation"), NextLocation);

	return EBTNodeResult::Succeeded;
}
