// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloWeaponBase.h"

// Sets default values
ATeloWeaponBase::ATeloWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(Root);

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

bool ATeloWeaponBase::GetAttackTraceData(FVector& OutStart, FVector& OutEnd) const
{
	if (!WeaponMesh) return false;
	if (TraceStartSocketName.IsNone() || TraceEndSocketName.IsNone()) return false;

	if (!WeaponMesh->DoesSocketExist(TraceStartSocketName) ||
		!WeaponMesh->DoesSocketExist(TraceEndSocketName))
	{
		return false;
	}

	OutStart = WeaponMesh->GetSocketLocation(TraceStartSocketName);
	OutEnd = WeaponMesh->GetSocketLocation(TraceEndSocketName);

	return true;
}

