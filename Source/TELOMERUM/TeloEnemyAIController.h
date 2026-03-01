// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TeloEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class TELOMERUM_API ATeloEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	ATeloEnemyAIController();

private:
	UPROPERTY(EditDefaultsOnly, Category = "EnemyAI")
	class UBehaviorTree* BehaviorTree;

protected:
	virtual void OnPossess(APawn* InPawn) override;
};
