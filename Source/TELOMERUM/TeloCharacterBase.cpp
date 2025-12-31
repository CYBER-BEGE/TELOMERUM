// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloCharacterBase.h"

// Sets default values
ATeloCharacterBase::ATeloCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//HealthComponent = CreateDefaultSubobject<UFrogHealthComponent>(TEXT("Health Component"));
}

// Called when the game starts or when spawned
void ATeloCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATeloCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATeloCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

