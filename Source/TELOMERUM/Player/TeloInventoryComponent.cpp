// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TeloInventoryComponent.h"

// Sets default values for this component's properties
UTeloInventoryComponent::UTeloInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false; // Tick 비활성화
}

// Called when the game starts
void UTeloInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UTeloInventoryComponent::AddItem(const FTeloInventoryItem& NewItem)
{
	// ItemID가 None인 경우 추가 실패
	if (NewItem.ItemID.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("[UTeloInventoryComponent] AddItem failed: ItemID is None"));
		return false;
	}

	// 이미 존재하는 아이템이면 개수만 증가
	for (FTeloInventoryItem& Item : Items)
	{
		if (Item.ItemID == NewItem.ItemID)
		{
			Item.Count += NewItem.Count;

			UE_LOG(LogTemp, Log, TEXT("[UTeloInventoryComponent] Stacked Item: %s / Count: %d"),
				*Item.ItemID.ToString(), Item.Count);

			return true;
		}
	}

	// 새로운 아이템이면 목록에 추가
	Items.Add(NewItem);

	UE_LOG(LogTemp, Log, TEXT("[UTeloInventoryComponent] Added New Item: %s / Count: %d"),
		*NewItem.ItemID.ToString(), NewItem.Count);

	return true;
}
