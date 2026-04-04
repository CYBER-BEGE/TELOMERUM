// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TeloWeaponBase.generated.h"

UCLASS()
class TELOMERUM_API ATeloWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeloWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	/* 메시 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;

	/* 공격 애니메이션 몽타주 - 반드시 BP에서 지정 */
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	class UAnimMontage* AttackMontage;

	/* 공격 판정 소켓 이름 */ 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	FName TraceSocketA = TEXT("SocketA");

	/* 공격 판정 소켓 이름(2소켓 전용) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	FName TraceSocketB = TEXT("SocketB");

	/* 2소켓 사용여부 */
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	bool bUseTwoSocketTrace = false;

	/* 무기 공격 속도 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	float WeaponSpeed = 1.0f;

	/* 무기 공격 사거리 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	float WeaponRange = 1.0f;

	/* 무기 공격 범위 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	float TraceRadius = 50.0f;

	/* 무기 공격력 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	float WeaponDamage = 1.0f;

public:
	/* 공격 애니메이션 몽타주 반환 */
	class UAnimMontage* GetAttackMontage() const { return AttackMontage; }
	/* 2소켓 사용여부 반환 */
	bool IsTwoSocketTrace() const { return bUseTwoSocketTrace; }
	/* 공격 소켓 위치를 넘겨주고 성공 여부를 반환 */
	bool GetTraceSoketVector(FVector& OutA, FVector& OutB) const;
};
