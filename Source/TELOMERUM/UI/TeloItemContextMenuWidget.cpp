// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TeloItemContextMenuWidget.h"
#include "Components/Button.h"

bool UTeloItemContextMenuWidget::Initialize()
{
	const bool bResult = Super::Initialize();

	if (UseButton)
	{
		UseButton->OnClicked.AddDynamic(this, &UTeloItemContextMenuWidget::HandleUseButtonClicked);
	}

	if (DropButton)
	{
		DropButton->OnClicked.AddDynamic(this, &UTeloItemContextMenuWidget::HandleDropButtonClicked);
	}

	return bResult;
}

void UTeloItemContextMenuWidget::SetItemData(const FTeloInventoryItem& InItemData)
{
	CachedItemData = InItemData;
}

void UTeloItemContextMenuWidget::HandleUseButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("[UTeloItemContextMenuWidget] Use Item: %s"),
		*CachedItemData.ItemID.ToString());
}

void UTeloItemContextMenuWidget::HandleDropButtonClicked()
{
	UE_LOG(LogTemp, Log, TEXT("[UTeloItemContextMenuWidget] Drop Item: %s"),
		*CachedItemData.ItemID.ToString());
}
