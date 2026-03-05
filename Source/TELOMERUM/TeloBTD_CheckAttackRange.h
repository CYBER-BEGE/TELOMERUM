// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "TeloBTD_CheckAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class TELOMERUM_API UTeloBTD_CheckAttackRange : public UBTDecorator
{
	GENERATED_BODY()
	
protected:
	// Sequence 실행 여부 결정
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory) const override;
};
