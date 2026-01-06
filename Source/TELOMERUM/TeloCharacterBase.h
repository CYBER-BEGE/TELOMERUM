// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TeloDamageable.h"

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
	/* Properties */

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

private:
	/* Properties */
	
	/* Take Damage */
	bool bIsDamageable = true;
	FTimerHandle DamageTimerHandle;

public:
	/* Interfaces */

	/* Take Damage */
	virtual void ApplyDamage(float Damage, AActor* DamageCauser, const FVector& DamageLocation, const FVector& DamageImpulse);
	
	/* Functions */

	/* Take Damage */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void DamageCooldown();

};
