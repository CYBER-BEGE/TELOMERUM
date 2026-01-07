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
	/* Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/* Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

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

	/* Player Controller */
	UPROPERTY()
	APlayerController* PlayerController;

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
	//bool bIsSliding = false;		// 웅크리고 있는지 여부

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

protected:
	void Landed(const FHitResult& Hit) override;

public:
	/* Animation State */
	UFUNCTION(BlueprintPure, Category = "Animation State")
	bool IsDashing() const { return bIsDashing; }
};
