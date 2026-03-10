// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TeloBTT_SetPatrolSpline.generated.h"

/**
 * 
 */
UCLASS()
class TELOMERUM_API UTeloBTT_SetPatrolSpline : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UTeloBTT_SetPatrolSpline();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
