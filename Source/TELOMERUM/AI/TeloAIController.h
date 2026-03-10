// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TeloAIController.generated.h"

/**
 * 
 */
UCLASS()
class TELOMERUM_API ATeloAIController : public AAIController
{
	GENERATED_BODY()

public:
	ATeloAIController();

protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	/** Variables **/
	UPROPERTY(EditDefaultsOnly, Category = "EnemyAI")
	class UBehaviorTree* BehaviorTree;

	class UBlackboardComponent* BlackboardComponent;

	UPROPERTY(VisibleAnywhere, Category = "EnemyAI")
	class UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, Category = "EnemyAI")
	class UAISenseConfig_Sight* SightConfig;

	/** Functions **/
	//virtual void OnPossess(APawn* InPawn) override;
	void DrawSightDebug();
	
	UFUNCTION()
	void OnTargetperceived(AActor* Actor, FAIStimulus Stimulus);
};
