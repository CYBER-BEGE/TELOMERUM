// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player/TeloInventoryComponent.h"
#include "TeloInventoryEntryWidget.generated.h"

/**
 * 
 */
UCLASS()
class TELOMERUM_API UTeloInventoryEntryWidget : public UUserWidget
{
	GENERATED_BODY()

	/* 아이템이 클릭됐을 때 부모 위젯에게 알리는 delegate 선언 */
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryEntryClicked, const FTeloInventoryItem&);
	
public:
	virtual bool Initialize() override;

	void SetItemData(const FTeloInventoryItem& ItemData);

	/* 아이템이 클릭됐을 때 부모 위젯에게 알리는 delegate */
	FOnInventoryEntryClicked OnInventoryEntryClicked;

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemNameText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemCountText;

	UPROPERTY(meta = (BindWidget))
	class UButton* EntryButton;

	/* 부모 위젯에게 선택을 알리는 delegate */
	UFUNCTION()
	void HandleEntryButtonClicked();

private:
	/* 클릭됐을 때 어떤 아이템인지 부모에게 넘겨주기 위해 저장 */
	FTeloInventoryItem CachedItemData;
};
