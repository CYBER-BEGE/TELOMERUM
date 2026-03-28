// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TeloInventoryEntryWidget.h"

#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"

#include "UI/TeloUISubsystem.h"

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

	if (ItemIconImage)
	{
		ItemIconImage->SetBrushFromTexture(ItemData.Icon);
	}

	if (ItemCountText)
	{
		ItemCountText->SetText(FText::AsNumber(ItemData.Count));
	}
}

bool UTeloInventoryEntryWidget::GetTooltipData(FTeloTooltipData& OutTooltipData) const
{
	OutTooltipData.Title = CachedItemData.ItemName;
	OutTooltipData.Description = CachedItemData.Description;
	OutTooltipData.bUseDescription = true;

	return true;
}

void UTeloInventoryEntryWidget::GetContextActions(TArray<FTeloContextAction>& OutActions) const
{
	FTeloContextAction UseAction;
	UseAction.ActionID = TEXT("Use");
	UseAction.Label = FText::FromString(TEXT("사용"));
	OutActions.Add(UseAction);

	FTeloContextAction DropAction;
	DropAction.ActionID = TEXT("Drop");
	DropAction.Label = FText::FromString(TEXT("버리기"));
	OutActions.Add(DropAction);
}

void UTeloInventoryEntryWidget::HandleContextAction(FName ActionID)
{
	if (ActionID == TEXT("Use"))
	{
		UE_LOG(LogTemp, Log, TEXT("[UTeloInventoryEntryWidget] Use Item: %s"),
			*CachedItemData.ItemID.ToString());
	}
	else if (ActionID == TEXT("Drop"))
	{
		UE_LOG(LogTemp, Log, TEXT("[UTeloInventoryEntryWidget] Drop Item: %s"),
			*CachedItemData.ItemID.ToString());
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

			UISubsystem->ShowContextFromSource(this, FVector2D(MouseX, MouseY)); // 클릭 시 컨텍스트 메뉴 표시
		}
	}
}

void UTeloInventoryEntryWidget::HandleEntryButtonHovered()
{
	if (ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
	{
		if (UTeloUISubsystem* UISubsystem = LocalPlayer->GetSubsystem<UTeloUISubsystem>())
		{
			UISubsystem->ShowTooltipFromSource(this);
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
