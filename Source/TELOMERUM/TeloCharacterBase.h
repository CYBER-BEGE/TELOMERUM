// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TeloCharacterBase.generated.h"

UCLASS()
class TELOMERUM_API ATeloCharacterBase : public ACharacter
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
	/* State */
	UPROPERTY(EditAnywhere, Category = "State")
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, Category = "State")
	float MoveSpeedScale = 1.0f;

	UPROPERTY(EditAnywhere, Category = "State")
	float JumpPowerScale = 1.0f;

	/* Component */
	//UPROPERTY(EditAnywhere)
	//UFrogHealthComponent* HealthComponent;

};
