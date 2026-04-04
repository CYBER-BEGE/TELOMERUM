// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Interfaces/TeloDamageable.h"

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

protected:
	/* 최대 HP */
	UPROPERTY(EditAnywhere, Category = "State")
	float MaxHP = 100.0f;

	/* 현재 HP */
	UPROPERTY(EditAnywhere, Category = "State")
	float NowHP = MaxHP;

	/* 이동 속도 스케일 값 */
	UPROPERTY(EditAnywhere, Category = "State")
	float MoveSpeedScale = 1.0f;

	/* 점프력 스케일 값 */
	UPROPERTY(EditAnywhere, Category = "State")
	float JumpPowerScale = 1.0f;

	/* 공격 속도 - 현재 미사용(애니메이션 재생속도에 영향을 줄듯) */
	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackSpeed = 1.5f;

	/* 공격 사거리 */
	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackRange = 100.0f;

	/* 공격 범위 */
	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackRadius = 50.0f;

	/* 공격력 */
	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackDamage = 10.0f;

	/* 넉백 임펄스 */
	UPROPERTY(EditAnywhere, Category = "Attack")
	float KnockbackImpulse = 250.0f;

	/* 에어본 임펄스 */
	UPROPERTY(EditAnywhere, Category = "Attack")
	float KnockupImpulse = 300.0f;

	/* ABP 접근용 State Getter: AnimNotify 사용하면서 공격 중 이동무시 외에 미사용 */
	UFUNCTION(BlueprintPure, Category = "Animation State")
	bool IsAttacking() const { return bIsAttacking; }

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

/* ==================== HP ==================== */

public:
	/* HP를 회복하고 성공 여부를 반환 */
	bool RecoverHP(float Amount);

public:
	/* 데미지 적용 함수(공격자 호출) */
	virtual void ApplyDamage(float Damage, AActor* DamageCauser, const FVector& DamageLocation, const FVector& DamageImpulse);
private:
	/* 데미지 적용 함수(피격자 호출) */
	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	/* 피격 쿨타임 */
	void DamageCooldown();
	/* 피격 디버그 드로잉 함수 */
	void DrawHitDebug(const FHitResult& Hit);

	/* 피격 State 변수: 피격 가능한 상태인가? */
	bool bIsDamageable = true;
	/* 피격 쿨타임 타이머 */
	FTimerHandle DamageTimerHandle;

/* ==================== Attack ==================== */

public:
	/* Attack 호출 함수 */
	void AttackRequest(AActor* Target);

protected:
	/* 공격 가능 조건 체크, 공격 State 설정 함수 */
	void StartAttack(AActor* Target);
public:
	/* 공격 State 초기화 함수 - 원래 protected 인데 AnimNoti가 사용중이라 임시 public 처리*/
	void EndAttack();

public:
	/* 공격 판정 시작 함수 */
	void StartAttackTrace();
	/* 공격 판정 함수 */
	void TickAttackTrace();
	/* 공격 판정 종료 함수 */
	void EndAttackTrace();

public:
	/* WeaponSocketName Getter - 삭제예정, BT 호환용 임시 유지 */
	FName GetAttackSocketName() const { return WeaponSocketName; }
	/* 실제 사거리를 리턴하는 함수(AttackRange가 아님) */
	float GetAttackDistance() const;
protected:
	/* 공격 판정 함수: 공격 적중 시 */
	virtual void HitActor(const FHitResult& HitResult);
	/* Target 방향으로 회전 */
	virtual void RotateToTarget(const AActor* Target);
private:
	/* 공격 히트박스 디버그 드로잉 함수 */
	void DrawAttackDebug(FVector TraceStart, FVector TraceEnd);
	/* 판정지점 받아오는 함수 */
	bool GetAttackTracePoint(FVector& TraceA, FVector& TraceB) const;

private:
	/* 공격 State: 공격 가능한 상태인가? */
	bool bCanAttack = true;
	/* 공격 State: 공격 중인가? */
	bool bIsAttacking = false;

	/* 공격 State: 공격 판정이 진행중인가? */
	bool bAttackTracing = false;
	/* 이전 판정지점A 벡터 */
	FVector PrevTraceA = FVector::ZeroVector;
	/* 이전 판정지점B 벡터 (2소켓 전용) */
	FVector PrevTraceB = FVector::ZeroVector;
	/* 이미 공격 판정된 액터 set (중복 데미지 방지용) */
	TSet<TWeakObjectPtr<AActor>> AlreadyHitActors;

public:
	/* 공격 종료 델리게이트 */
	FOnAttackEnd OnAttackEnd;

/* ==================== Weapon ==================== */

private:
	/* WeaponAttachComponent 부착 함수 */
	bool IsWeaponComponentAttached();
	/* 무기 스폰 함수 */
	class ATeloWeaponBase* SpawnWeapon(TSubclassOf<class ATeloWeaponBase> NewWeaponClass);
	/* 무기 장착 함수 */
	bool EquipWeapon(class ATeloWeaponBase* NewWeapon);

};
