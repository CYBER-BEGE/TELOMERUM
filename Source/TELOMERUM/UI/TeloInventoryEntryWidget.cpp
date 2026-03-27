// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TeloInventoryEntryWidget.h"
#include "UI/TeloUISubsystem.h"
#include "UI/TeloTooltipWidget.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"

bool UTeloInventoryEntryWidget::Initialize()
{
	const bool bResult = Super::Initialize();

	if (EntryButton)
	{
		EntryButton->OnClicked.AddDynamic(this, &UTeloInventoryEntryWidget::HandleEntryButtonClicked);

		EntryButton->OnHovered.AddDynamic(this, &UTeloInventoryEntryWidget::HandleEntryButtonHovered);
		EntryButton->OnUnhovered.AddDynamic(this, &UTeloInventoryEntryWidget::HandleEntryButtonUnhovered);
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

	if (ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
	{
		if (UTeloUISubsystem* UISubsystem = LocalPlayer->GetSubsystem<UTeloUISubsystem>())
		{
			APlayerController* PlayerController = GetOwningPlayer();
			if (!PlayerController)
			{
				return;
			}

			float MouseX = 0.0f;
			float MouseY = 0.0f;
			if (!PlayerController->GetMousePosition(MouseX, MouseY))
			{
				return;
			}

			UISubsystem->ShowItemContextMenu(CachedItemData, FVector2D(MouseX, MouseY));
		}
	}
}

void UTeloInventoryEntryWidget::HandleEntryButtonHovered()
{
	if (ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
	{
		if (UTeloUISubsystem* UISubsystem = LocalPlayer->GetSubsystem<UTeloUISubsystem>())
		{
			FTeloTooltipData TooltipData;
			TooltipData.Title = CachedItemData.ItemName;
			TooltipData.Description = CachedItemData.Description;
			TooltipData.bUseDescription = false;

			UISubsystem->ShowTooltip(TooltipData);
		}
	}
}

void UTeloInventoryEntryWidget::HandleEntryButtonUnhovered()
{
	if (ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
	{
		if (UTeloUISubsystem* UISubsystem = LocalPlayer->GetSubsystem<UTeloUISubsystem>())
		{
			UISubsystem->HideTooltip();
		}
	}
}
