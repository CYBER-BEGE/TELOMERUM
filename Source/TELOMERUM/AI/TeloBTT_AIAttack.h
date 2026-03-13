// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TeloBTT_AIAttack.generated.h"

/**
 * 
 */
UCLASS()
class TELOMERUM_API UTeloBTT_AIAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UTeloBTT_AIAttack();
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory) override;
};
