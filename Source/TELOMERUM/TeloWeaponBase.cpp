// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloWeaponBase.h"

// Sets default values
ATeloWeaponBase::ATeloWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	// 콜리전 끄기
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ATeloWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATeloWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ATeloWeaponBase::GetTraceSoketVector(FVector& OutA, FVector& OutB) const
{
	if (!WeaponMesh) return false;
	if (TraceSocketA.IsNone() || !WeaponMesh->DoesSocketExist(TraceSocketA)) return false;

	if (!bUseTwoSocketTrace) 
	{
		OutA = OutB = WeaponMesh->GetSocketLocation(TraceSocketA);
		return true;
	}

	if(TraceSocketB.IsNone() || !WeaponMesh->DoesSocketExist(TraceSocketB)) return false;

	OutA = WeaponMesh->GetSocketLocation(TraceSocketA);
	OutB = WeaponMesh->GetSocketLocation(TraceSocketB);
	return true;
}

