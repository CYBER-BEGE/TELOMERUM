// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloItemBase.h"

// Sets default values
ATeloItemBase::ATeloItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

// Called when the game starts or when spawned
void ATeloItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATeloItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ATeloItemBase::CanInteract(AActor* Interactor) const
{
	if (!bCanInteract) // 상호작용이 불가능한 상태라면
		return false;

	if (bIsPickedUp) // 이미 습득된 아이템이라면
		return false;

	if (Interactor == nullptr) // 상호작용하는 액터가 유효하지 않다면
		return false;

	return true;
}

void ATeloItemBase::Interact(AActor* Interactor)
{
	if (!CanInteract(Interactor))
		return;

	bIsPickedUp = true;

	UE_LOG(LogTemp, Warning, TEXT("[%s] 아이템을 습득했습니다."), *GetActorLabel());

	Destroy();
}
