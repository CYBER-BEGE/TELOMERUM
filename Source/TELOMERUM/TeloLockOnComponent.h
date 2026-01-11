// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TeloLockOnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TELOMERUM_API UTeloLockOnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTeloLockOnComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	/** 락온 토글 (없으면 찾고, 있으면 해제) */
	UFUNCTION(BlueprintCallable, Category = "LockOn")
	void ToggleLockOn();

	/** 강제 해제 */
	UFUNCTION(BlueprintCallable, Category = "LockOn")
	void ClearLockOn();

	/** 락온 상태 여부 */
	UFUNCTION(BlueprintPure, Category = "LockOn")
	bool IsLockOn() const { return bIsLockOn && Target.IsValid(); }

	/** 현재 타겟 반환 (없으면 nullptr) */
	UFUNCTION(BlueprintPure, Category = "LockOn")
	ATeloEnemyCharacter* GetTarget() const { return Target.Get(); }

	/** 현재 타겟의 락온 포인트 월드 위치 */
	UFUNCTION(BlueprintPure, Category = "LockOn")
	FVector GetTargetPointWorldLocation() const;

private:
	// ====== Tuning ======
	/** 적 탐색 반경 */
	UPROPERTY(EditAnywhere, Category = "LockOn|Search")
	float SearchRadius = 1500.0f;

	/** 카메라 전방 기준 최대 각도(도). 화면 중앙에 가까운 적만 락온 */
	UPROPERTY(EditAnywhere, Category = "LockOn|Search")
	float MaxLockOnAngleDeg = 60.0f;

	/** 락온 유지 최대 거리(초과 시 해제) */
	UPROPERTY(EditAnywhere, Category = "LockOn|Maintain")
	float BreakDistance = 2000.0f;

	/** LOS가 끊겨도 바로 해제하지 않고 버티는 시간(초) */
	UPROPERTY(EditAnywhere, Category = "LockOn|Maintain", meta = (ClampMin = "0.0"))
	float LoseSightGraceTime = 0.2f;

	/** 컨트롤러 회전 보정 속도 */
	UPROPERTY(EditAnywhere, Category = "LockOn|Camera", meta = (ClampMin = "0.0"))
	float RotationInterpSpeed = 12.0f;

	/** Pitch 제한 */
	UPROPERTY(EditAnywhere, Category = "LockOn|Camera")
	float MinPitch = -60.0f;

	UPROPERTY(EditAnywhere, Category = "LockOn|Camera")
	float MaxPitch = 35.0f;

	// ====== Runtime ======
	/** 락온 상태 여부 */
	UPROPERTY(Transient)
	bool bIsLockOn = false;

	// 현재 타겟
	TWeakObjectPtr<class ATeloEnemyCharacter> Target;

	// LOS 끊긴 시간 누적
	float LoseSightAccum = 0.0f;

	// 회전 모드 복구용 저장
	bool bPrevUseControllerYaw = false;
	bool bPrevOrientToMovement = true;

private:
	// 오너 캐릭터/컨트롤러 반환 헬퍼
	ACharacter* GetOwnerCharacter() const;
	APlayerController* GetOwnerPlayerController() const;

	// 최적 타겟 탐색
	ATeloEnemyCharacter* FindBestTarget() const;
	// 타겟에 대한 시야 확보 여부 반환
	bool HasLineOfSightToTarget(ATeloEnemyCharacter* InTarget) const;

	// 락온 시 캐릭터 이동 모드 적용/복구
	void ApplyLockOnMovementMode(bool bEnable);
	// 컨트롤러 회전을 타겟 쪽으로 업데이트
	void UpdateControlRotationToTarget(float DeltaTime);
};
