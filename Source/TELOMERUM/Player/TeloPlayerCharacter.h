// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TeloCharacterBase.h"
#include "TeloPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TELOMERUM_API ATeloPlayerCharacter : public ATeloCharacterBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	ATeloPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	/* 입력 처리용 컨트롤러 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	APlayerController* PlayerController;

	/* 카메라-캐릭터 간 거리 유지, 카메라 회전에 사용 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/* 실제 카메라 컴포넌트 (CameraBoom의 자식) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/* 적 락온 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UTeloLockOnComponent* LockOnComponent;

	/* 상호작용 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UTeloInteractComponent* InteractComponent;

	/* 인벤토리 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UTeloInventoryComponent* InventoryComponent;

private:
	/* 이동 InputAction */
	UPROPERTY(EditAnywhere, Category = "Input Action")
	class UInputAction* MoveAction;

	/* 카메라 이동, 회전 InputAction */
	UPROPERTY(EditAnywhere, Category = "Input Action")
	class UInputAction* LookAction;

	/* 점프 InputAction */
	UPROPERTY(EditAnywhere, Category = "Input Action")
	class UInputAction* JumpAction;

	/* 앉기 InputAction */
	UPROPERTY(EditAnywhere, Category = "Input Action")
	class UInputAction* CrouchAction;

	/* 대시 InputAction */
	UPROPERTY(EditAnywhere, Category = "Input Action")
	class UInputAction* DashAction;

	/* 공격 InputAction */
	UPROPERTY(EditAnywhere, Category = "Input Action")
	class UInputAction* AttackAction;

	/* 방어 InputAction */
	UPROPERTY(EditAnywhere, Category = "Input Action")
	class UInputAction* BlockAction;

	/* 락온 InputAction */
	UPROPERTY(EditAnywhere, Category = "Input Action")
	class UInputAction* LockOnAction;

	/* 상호작용 InputAction */
	UPROPERTY(EditAnywhere, Category = "Input Action")
	class UInputAction* InteractAction;

	/* 인벤토리 InputAction */
	UPROPERTY(EditAnywhere, Category = "Input Action")
	class UInputAction* InventoryAction;

private:
	/* Movement Component 초기화 함수 */
	void ResetMovementComps();

	/* Move */
	void MoveInput(const struct FInputActionValue& Value);
	void MoveInputEnd(const struct FInputActionValue& Value);
	void DoMove(float Right, float Forward);
	FVector2D InputVector; // 입력 벡터 저장 (X: Forward, Y: Right)

	/* Look */
	void LookInput(const struct FInputActionValue& Value);
	void DoLook(float Yaw, float Pitch);

	/* Jump */
	void DoJumpStart();
	void DoJumpEnd();

	/* Crouch */
	bool bCanCrouch = true; // 앉기 가능 여부

	void DoCrouchStart();
	void DoCrouchEnd();

	FVector CameraBoomDefaultRelativeLocation; // 카메라 붐의 기본 상대 위치

	// 앉을 때 카메라 붐의 Z 위치 오프셋
	UPROPERTY(EditAnywhere, Category = "Camera") 
	float CrouchCameraZOffset = 48.0f;

	/* Dash */
	FTimerHandle DashTimerHandle;	// 대시 쿨타임 타이머 핸들
	bool bIsDashing = false;		// 대시 중인지 여부
	bool bCanDash = true;			// 대시 가능 여부

	void DoDashStart();
	void DoDashEnd();
	void DashCooldown();			// 대시 쿨타임 완료 함수

	/* Lock On */
	void DoLockOn();

	/* 락온 시 카메라 Yaw 허용 범위 비율 (0.0 ~ 1.0) */
	UPROPERTY(EditAnywhere, Category = "LockOn|Camera", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float LockOnYawAllowRatio = 0.25f;

	/* true: 락온 시 특수한 동작, false : 통상 락온 모드 */
	//void ApplyLockOnMovementMode(bool bLockOn);

	/* Attack */
	/* 공격 입력 처리 함수 */
	void AttackInput();
	/* Target 방향으로 회전 */
	void RotateToTarget(const AActor* Target) override;

	/* Interact */
	void InteractInput();

	/* Inventory */
	void InventoryInput();

protected:
	virtual void Landed(const FHitResult& Hit) override;
	virtual bool CanJumpInternal_Implementation() const override;

public:
	/* Animation State */
	UFUNCTION(BlueprintPure, Category = "Animation State")
	bool IsDashing() const { return bIsDashing; }

	//UFUNCTION(BlueprintPure, Category = "Animation State")
	//bool IsSliding() const { return bIsSliding; }

public:
	// Returns CameraBoom subobject
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	// Returns FollowCamera subobject
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// Returns InventoryComponent
	FORCEINLINE class UTeloInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

	// Returns InteractComponent
	FORCEINLINE class UTeloInteractComponent* GetInteractComponent() const { return InteractComponent; }

public:
	/* 인벤토리 슬롯의 아이템 사용 시도 */
	bool TryUseItemAtSlot(int32 SlotIndex);

	/* 인벤토리 슬롯의 아이템을 월드에 드롭 시도 */
	bool TryDropItemAtSlot(int32 SlotIndex);
};
