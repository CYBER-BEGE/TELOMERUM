// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BlackboardComponent.h"

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TeloBTT_SearchPoint.generated.h"

/**
 * 
 */
UCLASS()
class TELOMERUM_API UTeloBTT_SearchPoint : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UTeloBTT_SearchPoint();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere)
	float SearchRadius = 600.f;
};
