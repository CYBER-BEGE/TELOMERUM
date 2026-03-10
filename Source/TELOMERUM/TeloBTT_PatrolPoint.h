// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TeloBTT_PatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class TELOMERUM_API UTeloBTT_PatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UTeloBTT_PatrolPoint();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "Patrol")
	bool bUsePingPong = false;
	
};
