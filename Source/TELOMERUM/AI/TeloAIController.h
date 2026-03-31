// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISense.h"
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

private:
	/** Variables **/
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	class UBlackboardComponent* BlackboardComponent;

	class UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UAISenseConfig_Damage* DamageConfig;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UAISenseConfig_Hearing* HearingConfig;

	// LastTargetLocation 보정 범위
	UPROPERTY(VisibleAnywhere, Category = "AI")
	FVector ProjectionExtent;

	/** Functions **/
	//virtual void OnPossess(APawn* InPawn) override;
	
	/* Perceive */
	UFUNCTION()
	void OnTargetperceived(AActor* Actor, FAIStimulus Stimulus);
	void UpdateTargetActor(AActor* Actor, const FVector& LastStimulusLocation);
	bool IsSensing(AActor* Actor, FAISenseID SenseID) const;
	
};
