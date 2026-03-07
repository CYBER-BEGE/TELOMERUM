// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TeloBTT_LookAround.generated.h"

/**
 * 
 */
UCLASS()
class TELOMERUM_API UTeloBTT_LookAround : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UTeloBTT_LookAround();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "LookAround")
	float YawOffset = 0.f;

};
