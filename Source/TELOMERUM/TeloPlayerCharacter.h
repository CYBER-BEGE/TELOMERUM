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
	/* Player Controller */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	APlayerController* PlayerController;

	/* Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/* Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/* Lock On Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UTeloLockOnComponent* LockOnComponent;

	/* Input Action */
	UPROPERTY(EditAnywhere, Category = "Input Action")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input Action")
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input Action")
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input Action")
	class UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, Category = "Input Action")
	class UInputAction* DashAction;

	UPROPERTY(EditAnywhere, Category = "Input Action")
	class UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = "Input Action")
	class UInputAction* BlockAction;

	UPROPERTY(EditAnywhere, Category = "Input Action")
	class UInputAction* LockOnAction;

	/* Movement Components */
	void ResetMovementComps();

	/* Move */
	void MoveInput(const struct FInputActionValue& Value);
	void DoMove(float Right, float Forward);

	/* Look */
	void LookInput(const struct FInputActionValue& Value);
	void DoLook(float Yaw, float Pitch);

	// Returns CameraBoom subobject
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	// Returns FollowCamera subobject
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/* Jump */
	void DoJumpStart();
	void DoJumpEnd();

	/* Crouch */
	void DoCrouchStart();
	void DoCrouchEnd();

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

	/* ture: 락온 시 특수한 동작, false : 통상 락온 모드 */
	void ApplyLockOnMovementMode(bool bLockOn);

	/* Attack */
	FTimerHandle AttackTimerHandle;	// 공격 쿨타임 타이머 핸들
	bool bIsAttacking = false;		// 공격 중인지 여부
	bool bCanAttack = true;			// 공격 가능 여부

	void DoAttackStart();
	void DoAttackEnd();

protected:
	void Landed(const FHitResult& Hit) override;

public:
	/* Animation State */
	UFUNCTION(BlueprintPure, Category = "Animation State")
	bool IsDashing() const { return bIsDashing; }

	//UFUNCTION(BlueprintPure, Category = "Animation State")
	//bool IsSliding() const { return bIsSliding; }

	UFUNCTION(BlueprintPure, Category = "Animation State")
	bool IsAttacking() const { return bIsAttacking; }

	/* Attack */
	virtual void HitActor(const FHitResult& HitResult);
};
