// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BlackboardComponent.h"

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TeloBTT_AIInvestigate.generated.h"

/**
 * 
 */
UCLASS()
class TELOMERUM_API UTeloBTT_AIInvestigate : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UTeloBTT_AIInvestigate();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "Investigate")
	float InvestigateRadius = 600.f; // 수색 반경
};
