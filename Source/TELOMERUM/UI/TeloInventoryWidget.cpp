// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TeloInventoryWidget.h"
#include "UI/TeloUISubsystem.h"
#include "Engine/LocalPlayer.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Input/Reply.h"
#include "InputCoreTypes.h"

bool UTeloInventoryWidget::Initialize()
{
	const bool bResult = Super::Initialize();

	SetIsFocusable(true); // 위젯이 포커스를 받을 수 있도록 설정

	return bResult;
}

FReply UTeloInventoryWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey PressedKey = InKeyEvent.GetKey();

	if (PressedKey == EKeys::Escape || PressedKey == EKeys::I)
	{
		if (ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
		{
			if (UTeloUISubsystem* UISubsystem = LocalPlayer->GetSubsystem<UTeloUISubsystem>())
			{
				UISubsystem->CloseInventory();
				return FReply::Handled();
			}
		}
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}
