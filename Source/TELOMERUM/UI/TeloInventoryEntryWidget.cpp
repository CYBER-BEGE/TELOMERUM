// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TeloInventoryEntryWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

bool UTeloInventoryEntryWidget::Initialize()
{
	const bool bResult = Super::Initialize();

	if (EntryButton)
	{
		EntryButton->OnClicked.AddDynamic(this, &UTeloInventoryEntryWidget::HandleEntryButtonClicked);
	}

	return bResult;
}

void UTeloInventoryEntryWidget::SetItemData(const FTeloInventoryItem& ItemData)
{
	CachedItemData = ItemData;

	//if (ItemNameText)
	//{
	//	ItemNameText->SetText(ItemData.ItemName);
	//}

	if (ItemIconImage)
	{
		ItemIconImage->SetBrushFromTexture(ItemData.Icon);
	}

	if (ItemCountText)
	{
		ItemCountText->SetText(FText::AsNumber(ItemData.Count));
	}
}

void UTeloInventoryEntryWidget::HandleEntryButtonClicked()
{
	OnInventoryEntryClicked.Broadcast(CachedItemData);
}
