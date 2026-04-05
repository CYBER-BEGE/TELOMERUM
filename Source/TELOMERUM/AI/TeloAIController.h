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
	/* AI BehaviorTree 설정 */
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	/* 시야 감지 설정 */
	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UAISenseConfig_Sight* SightConfig;

	/* 데미지 감지 설정 */
	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UAISenseConfig_Damage* DamageConfig;

	/* 기척 감지 설정 */
	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UAISenseConfig_Hearing* HearingConfig;

	/* LastTargetLocation 보정 범위 */
	UPROPERTY(VisibleAnywhere, Category = "AI")
	FVector ProjectionExtent;

	/* AI BlackboardComponent 캐시 변수 */
	class UBlackboardComponent* BlackboardComponent;

	/* AI AIPerceptionComponent 캐시 변수 */
	class UAIPerceptionComponent* AIPerceptionComponent;

	/* 감지 중일때 호출되는 함수*/
	UFUNCTION()
	void OnTargetperceived(AActor* Actor, FAIStimulus Stimulus);

	/* 감지중인 타겟 갱신하는 함수 */
	void UpdateTargetActor(AActor* Actor, const FVector& LastStimulusLocation);

	/* 감지 결과를 반환하는 함수 */
	bool IsSensing(AActor* Actor, FAISenseID SenseID) const;
	
};
