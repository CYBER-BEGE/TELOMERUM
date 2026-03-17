// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interfaces/TeloDamageable.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TeloCharacterBase.generated.h"

UCLASS()
class TELOMERUM_API ATeloCharacterBase : public ACharacter, public ITeloDamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATeloCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	/** Properties **/

	/* HP */
	UPROPERTY(EditAnywhere, Category = "State")
	float MaxHP = 100.0f;

	UPROPERTY(EditAnywhere, Category = "State")
	float NowHP = MaxHP;

	/* Movement */
	UPROPERTY(EditAnywhere, Category = "State")
	float MoveSpeedScale = 1.0f;

	UPROPERTY(EditAnywhere, Category = "State")
	float JumpPowerScale = 1.0f;

	/* Attack */

	// 공격 판정 소켓 이름 - 반드시 BP에서 지정
	UPROPERTY(EditAnywhere, Category = "Attack")
	FName AttackSocketName;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackSpeed = 1.5f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackRange = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackSize = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackDamage = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float KnockbackImpulse = 250.0f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float KnockupImpulse = 300.0f;

private:
	/** Variables **/

	/* Take Damage */
	bool bIsDamageable = true;
	FTimerHandle DamageTimerHandle;

	/* Attack */
	FTimerHandle AttackTimerHandle;	// 공격 쿨타임 타이머 핸들
	bool bCanAttack = true;			// 공격 가능 여부
	bool bIsAttacking = false;		// 공격 중인지 여부

	/** Functions **/

	/* Take Damage */
	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void DamageCooldown();

	/* Attack */
	void TraceAttack(FName DamageSourceBone); // 공격 시 소켓에서 정면으로 일정 거리까지 Sweep하여 공격 판정

	void DrawAttackDebug(FVector TraceStart, FVector TraceEnd);
	void DrawHitDebug(const FHitResult& Hit);

public:
	/** Interfaces **/

	/* Take Damage */
	virtual void ApplyDamage(float Damage, AActor* DamageCauser, const FVector& DamageLocation, const FVector& DamageImpulse);
	
	/** Functions **/

	/* Attack */
	FName GetAttackSocketName() const { return AttackSocketName; }
	float GetAttackDistance() const;
	void AttackRequest(AActor* Target);

	UFUNCTION(BlueprintPure, Category = "Animation State")
	bool IsAttacking() const { return bIsAttacking; }

protected:
	/** Variables **/

	/** Functions **/

	/* Attack */
	void DoAttack(AActor* Target);
	void DoAttackEnd();
	virtual void HitActor(const FHitResult& HitResult);
	virtual void RotateToTarget(const AActor* Target); // 타겟 방향으로 회전
};
