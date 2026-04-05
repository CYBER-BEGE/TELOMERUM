// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TeloContextEntryWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

bool UTeloContextEntryWidget::Initialize()
{
	const bool bResult = Super::Initialize();

	if (EntryButton)
	{
		EntryButton->OnClicked.AddDynamic(this, &UTeloContextEntryWidget::HandleEntryButtonClicked);
	}

	return bResult;
}

void UTeloContextEntryWidget::SetActionData(const FTeloContextAction& InActionData)
{
	CachedActionData = InActionData;

	if (EntryText)
	{
		EntryText->SetText(InActionData.Label);
	}

	if (EntryButton)
	{
		EntryButton->SetIsEnabled(InActionData.bEnabled);
	}
}

void UTeloContextEntryWidget::HandleEntryButtonClicked()
{
	OnEntryClicked.Broadcast(CachedActionData.ActionID);
}
