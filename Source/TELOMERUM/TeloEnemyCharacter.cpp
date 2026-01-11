// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloEnemyCharacter.h"

ATeloEnemyCharacter::ATeloEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// LockOnPoint 컴포넌트 생성 및 루트에 부착
	LockOnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("LockOnPoint"));
	LockOnPoint->SetupAttachment(RootComponent);

	// 초기 상태 설정
	MaxHealth = 100.0f;
}

// Called when the game starts or when spawned
void ATeloEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATeloEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ATeloEnemyCharacter::GetLockOnPointLocation() const
{
	return (LockOnPoint != nullptr) ? LockOnPoint->GetComponentLocation() : GetActorLocation();
}