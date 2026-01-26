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

	/** 타겟 마커 설정 */
	UFUNCTION(BlueprintCallable, Category = "LockOn")
	void SetMarker(ATeloEnemyCharacter* NewTarget);

	/** 락온 중 마우스 Yaw 입력을 “좌/우 타겟 전환”에 사용 */
	UFUNCTION(BlueprintCallable, Category = "LockOn")
	bool ConsumeYawForTargetSwitch(float YawInput);

	/** 좌/우 타겟 전환 (스크린 기준). +면 오른쪽, -면 왼쪽 */
	UFUNCTION(BlueprintCallable, Category = "LockOn")
	bool SwitchTargetHorizontal(float DirectionSign);

private:
	// ====== Tuning ======
	/** 적 탐색 반경 */
	UPROPERTY(EditAnywhere, Category = "LockOn|Search")
	float SearchRadius = 3000.0f;

	/** 카메라 전방 기준 최대 각도(도). 화면 중앙에 가까운 적만 락온 */
	UPROPERTY(EditAnywhere, Category = "LockOn|Search")
	float MaxLockOnAngleDeg = 60.0f;

	/** 락온 유지 최대 거리(초과 시 해제) */
	UPROPERTY(EditAnywhere, Category = "LockOn|Maintain")
	float BreakDistance = 3500.0f;

	/** LOS가 끊겨도 바로 해제하지 않고 버티는 시간(초) */
	UPROPERTY(EditAnywhere, Category = "LockOn|Maintain", meta = (ClampMin = "0.0"))
	float LoseSightGraceTime = 0.5f;

	/** 컨트롤러 회전 보정 속도 */
	UPROPERTY(EditAnywhere, Category = "LockOn|Camera", meta = (ClampMin = "0.0"))
	float RotationInterpSpeed = 12.0f;

	/** Pitch 제한 */
	UPROPERTY(EditAnywhere, Category = "LockOn|Camera")
	float MinPitch = -60.0f;

	UPROPERTY(EditAnywhere, Category = "LockOn|Camera")
	float MaxPitch = 35.0f;

	/** 마우스 좌/우 전환 누적 임계치 (감도에 따라 튜닝) */
	UPROPERTY(EditAnywhere, Category = "LockOn|Switch", meta = (ClampMin = "0.0"))
	float SwitchSnapThreshold = 35.0f;

	/** 전환 후 연속 전환 방지 쿨타임(초) */
	UPROPERTY(EditAnywhere, Category = "LockOn|Switch", meta = (ClampMin = "0.0"))
	float SwitchCooldownTime = 0.15f;

	/** 타겟 위치 오프셋 (월드 Z축 기준, 카메라 높이 조정용) */
	UPROPERTY(EditAnywhere, Category = "LockOn|Camera")
	float LookAtOffsetZ = -150.0f;

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

	/** 마우스 yaw 누적(스냅 전환용) */
	float SwitchYawAccum = 0.0f;

	/** 전환 쿨타임 남은 시간 */
	float SwitchCooldownRemain = 0.0f;

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
