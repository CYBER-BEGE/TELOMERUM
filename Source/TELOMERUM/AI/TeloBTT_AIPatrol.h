// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"

#include "TeloBTT_AIPatrol.generated.h"

/**
 * 
 */
UCLASS()
class TELOMERUM_API UTeloBTT_AIPatrol : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UTeloBTT_AIPatrol();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory) override;

private:
	/* 순찰 타입 - True: 왕복 순찰 사용, False: 순환 순찰 사용 */
	UPROPERTY(EditAnywhere, Category = "Patrol")
	bool bUsePingPong = false;
	
};
