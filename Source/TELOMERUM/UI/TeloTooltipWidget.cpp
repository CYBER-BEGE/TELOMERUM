// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TeloTooltipWidget.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "GameFramework/PlayerController.h"

void UTeloTooltipWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (GetVisibility() == ESlateVisibility::Visible) // Visible일 때만 위치 업데이트
	{
		UpdateTooltipPosition();
	}
}

void UTeloTooltipWidget::SetTooltipData(const FTeloTooltipData& InTooltipData)
{
	if (TitleText)
	{
		TitleText->SetText(InTooltipData.Title);
	}

	if (DescriptionText)
	{
		if (InTooltipData.bUseDescription)
		{
			DescriptionText->SetText(InTooltipData.Description);
			DescriptionText->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			DescriptionText->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UTeloTooltipWidget::UpdateTooltipPosition()
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

	SetPositionInViewport(FVector2D(MouseX, MouseY) + CursorOffset, true);
}
