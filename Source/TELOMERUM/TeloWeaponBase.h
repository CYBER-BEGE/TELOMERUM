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
	/* Components */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* WeaponMesh;

	// 공격 판정 시작점 소켓 이름
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	FName TraceSocketA = TEXT("SocketA");

	// 공격 판정 끝점 소켓 이름
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	FName TraceSocketB = TEXT("SocketB");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	bool bUseTwoSocketTrace = false;

	// 무기 전용 판정 반경
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	float TraceRadius = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	float WeaponSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	float WeaponDamage = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	float WeaponRange = 1.0f;

	// 공격 애니메이션 몽타주 - 반드시 BP에서 지정
	UPROPERTY(EditAnywhere, Category = "Attack")
	class UAnimMontage* AttackMontage;
public:
	bool GetAttackTraceData(FVector& OutStart, FVector& OutEnd) const;
	bool IsTwoSocketTrace() const { return bUseTwoSocketTrace; }

	class UAnimMontage* GetAttackMontage() const { return AttackMontage; }
};
