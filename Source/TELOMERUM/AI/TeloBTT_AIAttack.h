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
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory) override;

private:
	/** Variables **/

	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComponent = nullptr;

	UPROPERTY()
	class ATeloCharacterBase* CachedOwnerCharacter = nullptr;


	/** Functions **/	
	
	void ClearDelegate() const;
	void HandleAttackEnd();

};
