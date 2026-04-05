// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TeloInventorySlotWidget.h"

#include "Engine/LocalPlayer.h"
#include "Input/Reply.h"
#include "InputCoreTypes.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "Components/Border.h"
#include "Components/Overlay.h"

#include "UI/TeloUISubsystem.h"
#include "UI/TeloInventoryEntryWidget.h"
#include "UI/TeloDragDropOperation.h"


/* ===================== Widget Event Handlers ==================== */

FReply UTeloInventorySlotWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	const FKey PressedButton = InMouseEvent.GetEffectingButton();

	if (PressedButton != EKeys::LeftMouseButton)
	{
		return Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
	}

	// 이번 클릭은 아직 드래그가 시작되지 않은 상태
	bDragStartedThisPress = false;

	// 먼저 부모 인벤토리 위젯에 현재 슬롯이 클릭됐음을 알림
	OnSlotLeftClicked.Broadcast(CachedSlotIndex);

	// 아이템이 있는 슬롯이면 드래그 감지를 등록
	if (EntryWidgetInstance && !CachedSlotData.IsEmpty())
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}

	// 빈 슬롯은 선택 처리만 하고 종료
	return FReply::Handled();
}

/* 좌클릭을 뗐을 때, 실제 드래그가 아니었다면 컨텍스트 메뉴를 표시한다 */
FReply UTeloInventorySlotWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	const FKey PressedButton = InMouseEvent.GetEffectingButton();

	if (PressedButton != EKeys::LeftMouseButton)
	{
		return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
	}

	// 드래그가 시작되지 않았고, 아이템이 있는 슬롯이면 컨텍스트 메뉴 표시
	if (!bDragStartedThisPress && EntryWidgetInstance)
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

void UTeloInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (!EntryWidgetInstance || CachedSlotData.IsEmpty())
	{
		return;
	}

	bDragStartedThisPress = true;

	UTeloDragDropOperation* DragOperation = NewObject<UTeloDragDropOperation>();
	if (!DragOperation)
	{
		return;
	}

	DragOperation->SourceSlotIndex = CachedSlotIndex;
	DragOperation->DraggedItemData = CachedSlotData.ItemData;
	DragOperation->Pivot = EDragPivot::MouseDown;

	// 드래그 비주얼은 현재 아이템 위젯 클래스를 복제해서 사용
	UTeloInventoryEntryWidget* DragVisual = CreateWidget<UTeloInventoryEntryWidget>(GetOwningPlayer(), EntryWidgetInstance->GetClass());

	if (DragVisual)
	{
		DragVisual->SetItemData(CachedSlotData.ItemData);	// 드래그 비주얼에도 아이템 데이터 설정
		DragOperation->DefaultDragVisual = DragVisual;		// 드래그 비주얼로 설정
	}

	OutOperation = DragOperation;
}

bool UTeloInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	const UTeloDragDropOperation* DragOperation = Cast<UTeloDragDropOperation>(InOperation);

	if (!DragOperation)
	{
		return false;
	}

	OnSlotDropped.Broadcast(DragOperation->SourceSlotIndex, CachedSlotIndex); // 슬롯 이동이 발생했음을 부모 인벤토리 위젯에 알림
	return true;
}


/* ==================== Inventory Slot Setup ==================== */

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
	EntryWidgetInstance->SetSlotIndex(CachedSlotIndex);
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


/* ==================== Internal Functions ==================== */

UTeloUISubsystem* UTeloInventorySlotWidget::GetTeloUISubsystem() const
{
	if (ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
	{
		return LocalPlayer->GetSubsystem<UTeloUISubsystem>();
	}

	return nullptr;
}