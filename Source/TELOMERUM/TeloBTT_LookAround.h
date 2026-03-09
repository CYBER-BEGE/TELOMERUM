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
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:

	float StartYaw = 0.f;
	float TargetYaw = 0.f;

	int32 Phase = 0;
	
	float NormalizeYaw(float Yaw) const;
	bool IsYawReached(float CurrentYaw, float InTargetYaw) const;

	UPROPERTY(EditAnywhere)
	float RotationSpeed = 90.f;

	UPROPERTY(EditAnywhere)
	float LookAngle = 60.f;

};
