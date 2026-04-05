// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"

#include "TeloBTD_CheckAttackDistance.generated.h"

/**
 * 
 */
UCLASS()
class TELOMERUM_API UTeloBTD_CheckAttackDistance : public UBTDecorator
{
	GENERATED_BODY()
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory) const override;
};
