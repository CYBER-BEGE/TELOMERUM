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
	/* 이 Task가 소속된 BehaviorTreeComponent */
	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComponent = nullptr;

	/* 이 Task를 실행중인 캐릭터 */
	UPROPERTY()
	class ATeloCharacterBase* CachedOwnerCharacter = nullptr;
	
	/* 델리게이트 초기화 함수 */
	void ClearDelegate() const;

	/* 공격 종료 수신 시 호출되는 함수 */
	void HandleAttackEnd();

};
