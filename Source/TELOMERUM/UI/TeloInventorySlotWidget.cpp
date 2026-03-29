// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TeloInventorySlotWidget.h"

#include "Engine/LocalPlayer.h"
#include "Input/Reply.h"
#include "InputCoreTypes.h"
#include "GameFramework/PlayerController.h"

#include "Components/Border.h"
#include "Components/Overlay.h"

#include "UI/TeloUISubsystem.h"
#include "UI/TeloInventoryEntryWidget.h"

void UTeloInventorySlotWidget::SetSlotIndex(int32 InSlotIndex)
{
	CachedSlotIndex = InSlotIndex;
}

void UTeloInventorySlotWidget::SetEntryWidgetClass(TSubclassOf<UTeloInventoryEntryWidget> InEntryWidgetClass)
{
	InventoryEntryWidgetClass = InEntryWidgetClass;
}

void UTeloInventorySlotWidget::RefreshSlot(const FTeloInventorySlot& InSlotData)
{
	CachedSlotData = InSlotData;

	if (!ItemContainer)
	{
		return;
	}

	ItemContainer->ClearChildren();
	EntryWidgetInstance = nullptr;

	if (CachedSlotData.IsEmpty())
	{
		return;
	}

	if (!InventoryEntryWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UTeloInventorySlotWidget] InventoryEntryWidgetClass is NULL"));
		return;
	}

	EntryWidgetInstance = CreateWidget<UTeloInventoryEntryWidget>(GetOwningPlayer(), InventoryEntryWidgetClass);
	if (!EntryWidgetInstance)
	{
		return;
	}

	EntryWidgetInstance->SetItemData(CachedSlotData.ItemData);
	ItemContainer->AddChild(EntryWidgetInstance);
}

void UTeloInventorySlotWidget::SetSelected(bool bInSelected)
{
	if (!SlotBorder)
	{
		return;
	}

	SlotBorder->SetBrushColor(
		bInSelected
		? FLinearColor(1.0f, 0.8f, 0.2f, 1.0f)
		: FLinearColor::White);
}

FReply UTeloInventorySlotWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	const FKey PressedButton = InMouseEvent.GetEffectingButton();

	// 우클릭은 사용하지 않으므로 아무 처리도 하지 않음
	if (PressedButton != EKeys::LeftMouseButton)
	{
		return Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
	}

	// 부모 인벤토리 위젯에게 현재 슬롯이 클릭됐음을 알림
	OnSlotLeftClicked.Broadcast(CachedSlotIndex);

	// 아이템이 들어있는 슬롯이면 컨텍스트 메뉴 표시
	if (EntryWidgetInstance)
	{
		if (UTeloUISubsystem* UISubsystem = GetTeloUISubsystem())
		{
			APlayerController* PlayerController = GetOwningPlayer();
			if (!PlayerController)
			{
				return FReply::Handled();
			}

			float MouseX = 0.0f;
			float MouseY = 0.0f;
			if (!PlayerController->GetMousePosition(MouseX, MouseY))
			{
				return FReply::Handled();
			}

			UISubsystem->ShowContextFromSource(
				EntryWidgetInstance,
				FVector2D(MouseX, MouseY));
		}
	}

	return FReply::Handled();
}

UTeloUISubsystem* UTeloInventorySlotWidget::GetTeloUISubsystem() const
{
	if (ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
	{
		return LocalPlayer->GetSubsystem<UTeloUISubsystem>();
	}

	return nullptr;
}