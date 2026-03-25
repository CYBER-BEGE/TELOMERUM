// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/TeloItemBase.h"
#include "Player/TeloPlayerCharacter.h"
#include "Player/TeloInventoryComponent.h"

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
	
	if (ItemID.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] ItemID is NULL"), *GetActorLabel());
	}
	if (ItemName.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] ItemName is empty"), *GetActorLabel());
	}
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
	{
		return;
	}

	ATeloPlayerCharacter* PlayerCharacter = Cast<ATeloPlayerCharacter>(Interactor);
	if (!PlayerCharacter)
	{
		return;
	}

	UTeloInventoryComponent* InventoryComponent = PlayerCharacter->GetInventoryComponent();
	if (!InventoryComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] InventoryComponent is NULL"), *GetActorLabel());
		return;
	}

	FTeloInventoryItem NewItem;
	NewItem.ItemID = ItemID;
	NewItem.ItemName = ItemName;
	NewItem.Description = ItemDescription;
	NewItem.Count = ItemCount;

	if (!InventoryComponent->AddItem(NewItem)) // 인벤토리에 아이템 추가 실패
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] Failed to add item to inventory"), *GetActorLabel());
		return;
	}

	bIsPickedUp = true; // 아이템이 습득된 상태로 변경

	UE_LOG(LogTemp, Warning, TEXT("[%s] 아이템을 습득했습니다. ItemID: %s / Count: %d"),
		*GetActorLabel(),
		*ItemID.ToString(),
		ItemCount);

	Destroy();
}

FText ATeloItemBase::GetInteractText() const
{
	return InteractText;
}
