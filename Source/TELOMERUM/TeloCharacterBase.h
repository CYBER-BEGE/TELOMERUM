// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interfaces/TeloDamageable.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TeloCharacterBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEnd);

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

public:
	/* ==================== External API ==================== */

	/* 데미지 적용시키는 함수(공격자 호출) */
	virtual void ApplyDamage(float Damage, AActor* DamageCauser, const FVector& DamageLocation, const FVector& DamageImpulse);

	/* AI의 Attack 호출 함수 */
	void AttackRequest(AActor* Target);

	/* 공격 판정 함수 */
	void StartAttackTrace();
	void TickAttackTrace();
	void EndAttackTrace();

	/* ==================== Query ==================== */

	float GetAttackDistance() const;
	FName GetAttackSocketName() const { return WeaponSocketName; } // BT 호환용 임시 유지

	UFUNCTION(BlueprintPure, Category = "Animation State")
	bool IsAttacking() const { return bIsAttacking; }
	/* └> AnimNotify 사용하면서 변경점: 외부 미사용 시 protected로 옮기고 UFUNCTION 제거 */

	/* 공격 종료 델리게이트 */
	FOnAttackEnd OnAttackEnd;

protected:
	/* ==================== Movement ==================== */

	/* 이동 속도 스케일 값 */
	UPROPERTY(EditAnywhere, Category = "State")
	float MoveSpeedScale = 1.0f;

	/* 점프력 스케일 값 */
	UPROPERTY(EditAnywhere, Category = "State")
	float JumpPowerScale = 1.0f;

	/* ==================== HP ==================== */

	/* 최대 HP */
	UPROPERTY(EditAnywhere, Category = "State")
	float MaxHP = 100.0f;

	/* 현재 HP */
	UPROPERTY(EditAnywhere, Category = "State")
	float NowHP = MaxHP;

	/* ==================== Weapon ==================== */

	/* 무기 장착위치 소켓 이름 (반드시 BP에서 지정) */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponSocketName;

	/* 무기 장착 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	class USceneComponent* WeaponAttachComponent;

	/* 무기 클래스 */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class ATeloWeaponBase> WeaponClass;

	/* 런타임에 실제로 장착중인 무기 인스턴스 */
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	class ATeloWeaponBase* WeaponInstance;

	/* ==================== Attack ==================== */

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

	/* 공격 가능 조건 체크, 공격 State 설정 함수 */
	void StartAttack(AActor* Target);
	
	/* 공격 State 초기화 함수 */
	void EndAttack();

	/* ==================== Override ==================== */

	virtual void HitActor(const FHitResult& HitResult);
	virtual void RotateToTarget(const AActor* Target); // 타겟 방향으로 회전

private:
	/* ==================== State ==================== */

	/* Damage */
	bool bIsDamageable = true;
	FTimerHandle DamageTimerHandle;

	/* Attack */
	bool bCanAttack = true;			// 공격 가능 여부
	bool bIsAttacking = false;		// 공격 중인지 여부
	bool bAttackTracing = false;	// 공격 판정 중인지 여부

	/* Attack Trace */
	FVector PrevTraceA = FVector::ZeroVector;
	FVector PrevTraceB = FVector::ZeroVector;
	TSet<TWeakObjectPtr<AActor>> AlreadyHitActors;

	/* ==================== Damage Internal ==================== */

	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void DamageCooldown();

	/* ==================== Weapon Internal ==================== */

	/* WeaponAttachComponent 부착 함수 */
	bool IsWeaponComponentAttached();

	/* 무기 스폰 함수 */
	class ATeloWeaponBase* SpawnWeapon(TSubclassOf<class ATeloWeaponBase> NewWeaponClass);

	/* 무기 장착 함수 */
	bool EquipWeapon(class ATeloWeaponBase* NewWeapon);

	/* ==================== Attack Internal ==================== */

	void DrawAttackDebug(FVector TraceStart, FVector TraceEnd);
	void DrawHitDebug(const FHitResult& Hit);

	bool GetAttackTracePoints(FVector& TraceA, FVector& TraceB) const;
};
