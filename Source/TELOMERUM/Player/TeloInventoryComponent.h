// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/TeloItemTypes.h"
#include "TeloInventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FTeloInventoryItem
{
	GENERATED_BODY()

	/* 아이템 고유 ID */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FName ItemID = NAME_None;

	/* 아이템 이름 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FText ItemName = FText::GetEmpty();

	/* 아이템 설명 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FText Description = FText::GetEmpty();

	/* 아이템 아이콘 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	class UTexture2D* Icon = nullptr;

	/* 아이템 개수 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 Count = 1;

	/* 사용 가능한 아이템인지 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	bool bUsable = false;

	/* 사용 후 아이템을 소비할지 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	bool bConsumeOnUse = true;

	/* 아이템 사용 타입 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	ETeloItemUseType UseType = ETeloItemUseType::None;

	/* 아이템 사용 수치 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	float UseValue = 0.0f;
};

USTRUCT(BlueprintType)
struct FTeloInventorySlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FTeloInventoryItem ItemData;

	/* 현재 슬롯이 비어있는지 확인 */
	bool IsEmpty() const { return ItemData.ItemID.IsNone() || ItemData.Count <= 0; }

	/* 현재 슬롯을 빈 슬롯으로 초기화 */
	void Clear() { ItemData = FTeloInventoryItem(); }
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TELOMERUM_API UTeloInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	/* 인벤토리 데이터가 변경되었을 때 알리는 델리게이트 */
	DECLARE_MULTICAST_DELEGATE(FOnInventoryChanged);

public:	
	// Sets default values for this component's properties
	UTeloInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	/* 인벤토리 전체 슬롯 개수 */
	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (ClampMin = "1"))
	int32 SlotCount = 20;

	/* 인벤토리 슬롯 배열 */
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<FTeloInventorySlot> Slots;

public:
	/* 인벤토리 슬롯 배열을 SlotCount만큼 초기화 */
	void InitializeSlots();

	/* 아이템 추가 (같은 아이템이 있으면 스택, 없으면 빈 슬롯에 추가) */
	bool AddItem(const FTeloInventoryItem& NewItem);

	/* 슬롯 간 아이템 이동/교체 */
	bool MoveSlot(int32 FromIndex, int32 ToIndex);

	/* 슬롯끼리 강제 교체 */
	bool SwapSlot(int32 SlotA, int32 SlotB);

	/* 슬롯 배열 반환 */
	const TArray<FTeloInventorySlot>& GetSlots() const { return Slots; }

	/* 슬롯 개수 반환 */
	int32 GetSlotCount() const { return Slots.Num(); }

	/* 유효한 슬롯 인덱스인지 확인 */
	bool IsValidSlotIndex(int32 Index) const;

	/* 슬롯이 비어있는지 확인 */
	bool IsSlotEmpty(int32 Index) const;

	/* 실제 아이템이 들어있는 슬롯 수 반환 */
	int32 GetItemCount() const;

	/* 슬롯의 아이템을 지정한 수량만큼 소비 */
	bool ConsumeItemAtSlot(int32 SlotIndex, int32 Amount = 1);

public:
	/* 인벤토리 데이터가 변경되었을 때 UI 등에 알리기 위한 델리게이트 */
	FOnInventoryChanged OnInventoryChanged;

private:
	/* 스택 가능한 슬롯 인덱스 탐색 */
	int32 FindStackableSlotIndex(const FTeloInventoryItem& NewItem) const;

	/* 빈 슬롯 인덱스 탐색 */
	int32 FindEmptySlotIndex() const;
};
