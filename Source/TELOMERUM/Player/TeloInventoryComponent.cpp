// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TeloInventoryComponent.h"

#include "Engine/Texture2D.h"


/* ==================== ActorComponent Lifecycle ==================== */

// Sets default values for this component's properties
UTeloInventoryComponent::UTeloInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false; // Tick 비활성화
}

// Called when the game starts
void UTeloInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeSlots();
}


/* ===================== Inventory Setup ==================== */

void UTeloInventoryComponent::InitializeSlots()
{
	if (SlotCount <= 0)
	{
		SlotCount = 20;
	}

	Slots.SetNum(SlotCount);
}


/* ===================== Inventory Setup ==================== */

/* 아이템 추가 (스택 가능하면 스택, 아니면 빈 슬롯에 추가) */
bool UTeloInventoryComponent::AddItem(const FTeloInventoryItem& NewItem)
{
	// ItemID가 None이면 추가 실패
	if (NewItem.ItemID.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("[UTeloInventoryComponent] AddItem failed: ItemID is None"));
		return false;
	}

	// Count가 1 이상이어야 추가 가능
	if (Slots.IsEmpty())
	{
		InitializeSlots();
	}

	// 스택 가능한 슬롯이 있으면 스택, 없으면 빈 슬롯에 추가
	const int32 StackableSlotIndex = FindStackableSlotIndex(NewItem);
	if (StackableSlotIndex != INDEX_NONE)
	{
		Slots[StackableSlotIndex].ItemData.Count += NewItem.Count;

		UE_LOG(LogTemp, Log, TEXT("[UTeloInventoryComponent] Stacked Item: %s / Count: %d"),
			*NewItem.ItemID.ToString(),
			Slots[StackableSlotIndex].ItemData.Count);

		// 인벤토리 데이터 변경 알림
		OnInventoryChanged.Broadcast();

		return true;
	}

	// 빈 슬롯 탐색
	const int32 EmptySlotIndex = FindEmptySlotIndex();
	if (EmptySlotIndex == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UTeloInventoryComponent] AddItem failed: Inventory is full"));
		return false;
	}

	// 빈 슬롯에 아이템 추가
	Slots[EmptySlotIndex].ItemData = NewItem;

	UE_LOG(LogTemp, Log, TEXT("[UTeloInventoryComponent] Added New Item: %s / Slot: %d / Count: %d"),
		*NewItem.ItemID.ToString(),
		EmptySlotIndex,
		NewItem.Count);

	// 인벤토리 데이터 변경 알림
	OnInventoryChanged.Broadcast();

	return true;
}

bool UTeloInventoryComponent::MoveSlot(int32 FromIndex, int32 ToIndex)
{
	if (!IsValidSlotIndex(FromIndex) || !IsValidSlotIndex(ToIndex))
	{
		return false;
	}

	if (FromIndex == ToIndex)
	{
		return true;
	}

	FTeloInventorySlot& FromSlot = Slots[FromIndex];
	FTeloInventorySlot& ToSlot = Slots[ToIndex];

	if (FromSlot.IsEmpty())
	{
		return false;
	}

	// 목적지가 빈 슬롯이면 이동
	if (ToSlot.IsEmpty())
	{
		ToSlot.ItemData = FromSlot.ItemData;
		FromSlot.Clear();

		// 인벤토리 데이터 변경 알림
		OnInventoryChanged.Broadcast();

		return true;
	}

	// 같은 아이템이면 스택
	if (ToSlot.ItemData.ItemID == FromSlot.ItemData.ItemID)
	{
		ToSlot.ItemData.Count += FromSlot.ItemData.Count;
		FromSlot.Clear();

		// 인벤토리 데이터 변경 알림
		OnInventoryChanged.Broadcast();

		return true;
	}

	// 다른 아이템이면 교체
	return SwapSlot(FromIndex, ToIndex);
}

bool UTeloInventoryComponent::SwapSlot(int32 SlotA, int32 SlotB)
{
	if (!IsValidSlotIndex(SlotA) || !IsValidSlotIndex(SlotB))
	{
		return false;
	}

	Slots.Swap(SlotA, SlotB);

	// 인벤토리 데이터 변경 알림
	OnInventoryChanged.Broadcast();

	return true;
}

bool UTeloInventoryComponent::ConsumeItemAtSlot(int32 SlotIndex, int32 Amount)
{
	if (!IsValidSlotIndex(SlotIndex))
	{
		return false;
	}

	if (Amount <= 0)
	{
		return false;
	}

	FTeloInventorySlot& Slot = Slots[SlotIndex];
	if (Slot.IsEmpty())
	{
		return false;
	}

	if (Slot.ItemData.Count < Amount)
	{
		return false;
	}

	Slot.ItemData.Count -= Amount;

	/* 개수가 0 이하가 되면 슬롯 비우기 */
	if (Slot.ItemData.Count <= 0)
	{
		Slot.Clear();
	}

	// 인벤토리 데이터 변경 알림
	OnInventoryChanged.Broadcast();

	return true;
}

bool UTeloInventoryComponent::IsValidSlotIndex(int32 Index) const
{
	return Slots.IsValidIndex(Index);
}

bool UTeloInventoryComponent::IsSlotEmpty(int32 Index) const
{
	if (!IsValidSlotIndex(Index))
	{
		return true;
	}

	return Slots[Index].IsEmpty();
}

int32 UTeloInventoryComponent::GetItemCount() const
{
	int32 OccupiedCount = 0;

	for (const FTeloInventorySlot& Slot : Slots)
	{
		if (!Slot.IsEmpty())
		{
			++OccupiedCount;
		}
	}

	return OccupiedCount;
}


/* ============== Inventory Internal Functions ==================== */

int32 UTeloInventoryComponent::FindStackableSlotIndex(const FTeloInventoryItem& NewItem) const
{
	for (int32 Index = 0; Index < Slots.Num(); ++Index)
	{
		const FTeloInventorySlot& Slot = Slots[Index];

		if (!Slot.IsEmpty() && Slot.ItemData.ItemID == NewItem.ItemID)
		{
			return Index;
		}
	}

	return INDEX_NONE;
}

int32 UTeloInventoryComponent::FindEmptySlotIndex() const
{
	for (int32 Index = 0; Index < Slots.Num(); ++Index)
	{
		if (Slots[Index].IsEmpty())
		{
			return Index;
		}
	}

	return INDEX_NONE;
}
