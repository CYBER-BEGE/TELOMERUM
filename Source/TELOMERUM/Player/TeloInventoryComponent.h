// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TeloInventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FTeloInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FName ItemID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FText ItemName = FText::GetEmpty();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FText Description = FText::GetEmpty();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 Count = 1;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TELOMERUM_API UTeloInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTeloInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	/* 인벤토리 아이템 목록 */
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<FTeloInventoryItem> Items;

public:
	/* 아이템 추가 (이미 존재하는 아이템이면 개수만 증가) */
	bool AddItem(const FTeloInventoryItem& NewItem);

	/* 아이템 목록 반환 */
	const TArray<FTeloInventoryItem>& GetItems() const { return Items; }

	/* 아이템 개수 반환 */
	int32 GetItemCount() const { return Items.Num(); }
		
};
