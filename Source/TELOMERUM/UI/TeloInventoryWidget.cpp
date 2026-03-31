// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TeloInventoryWidget.h"

#include "InputCoreTypes.h"

#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

#include "Player/TeloPlayerCharacter.h"
#include "Player/TeloInventoryComponent.h"
#include "UI/TeloUISubsystem.h"
#include "UI/TeloInventoryEntryWidget.h"
#include "UI/TeloInventorySlotWidget.h"

FReply UTeloInventoryWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey PressedKey = InKeyEvent.GetKey();

	if (PressedKey == EKeys::Escape || PressedKey == EKeys::I)
	{
		if (UTeloUISubsystem* UISubsystem = GetTeloUISubsystem())
		{
			UISubsystem->CloseInventory(); // 인벤토리 UI 닫기
			return FReply::Handled();
		}
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UTeloInventoryWidget::OnScreenOpened()
{
	Super::OnScreenOpened();

	// 슬롯 위젯 생성 후 데이터 갱신
	SelectedSlotIndex = INDEX_NONE;
	BuildSlotWidgets();
	RefreshInventory();
}

void UTeloInventoryWidget::BuildSlotWidgets()
{
	if (!SlotGridPanel)
	{
		return;
	}

	ATeloPlayerCharacter* PlayerCharacter = Cast<ATeloPlayerCharacter>(GetOwningPlayerPawn());
	if (!PlayerCharacter)
	{
		return;
	}

	UTeloInventoryComponent* InventoryComponent = PlayerCharacter->GetInventoryComponent();
	if (!InventoryComponent)
	{
		return;
	}

	if (!InventorySlotWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UTeloInventoryWidget] InventorySlotWidgetClass is NULL"));
		return;
	}

	// 기존 슬롯 위젯 제거
	SlotGridPanel->ClearChildren();
	SlotWidgets.Reset();

	// 인벤토리 슬롯 개수만큼 슬롯 위젯 생성
	const int32 SlotCount = InventoryComponent->GetSlotCount();
	for (int32 SlotIndex = 0; SlotIndex < SlotCount; ++SlotIndex)
	{
		UTeloInventorySlotWidget* SlotWidget = CreateWidget<UTeloInventorySlotWidget>(GetOwningPlayer(), InventorySlotWidgetClass);

		// 슬롯 위젯 생성 실패 시 다음 슬롯으로 넘어감
		if (!SlotWidget)
		{
			continue;
		}

		// 슬롯 위젯 초기화
		SlotWidget->SetSlotIndex(SlotIndex);
		SlotWidget->SetEntryWidgetClass(InventoryEntryWidgetClass);
		SlotWidget->OnSlotLeftClicked.AddUObject(this, &UTeloInventoryWidget::HandleSlotLeftClicked);	// 슬롯 좌클릭 이벤트 바인딩
		SlotWidget->OnSlotDropped.AddUObject(this, &UTeloInventoryWidget::HandleSlotDropped);			// 슬롯 드래그 드롭 이벤트 바인딩

		const int32 Row = SlotIndex / InventoryColumnCount;
		const int32 Column = SlotIndex % InventoryColumnCount;

		// UniformGridPanel에 슬롯 위젯 추가
		if (UUniformGridSlot* GridSlot = SlotGridPanel->AddChildToUniformGrid(SlotWidget, Row, Column))
		{
			GridSlot->SetHorizontalAlignment(HAlign_Fill);
			GridSlot->SetVerticalAlignment(VAlign_Fill);
		}

		SlotWidgets.Add(SlotWidget);
	}
}

void UTeloInventoryWidget::RefreshInventory()
{
	if (!SlotGridPanel)
	{
		return;
	}

	ATeloPlayerCharacter* PlayerCharacter = Cast<ATeloPlayerCharacter>(GetOwningPlayerPawn());
	if (!PlayerCharacter)
	{
		return;
	}

	UTeloInventoryComponent* InventoryComponent = PlayerCharacter->GetInventoryComponent();
	if (!InventoryComponent)
	{
		return;
	}

	// 인벤토리 슬롯 데이터 가져오기
	const TArray<FTeloInventorySlot>& Slots = InventoryComponent->GetSlots();
	if (SlotWidgets.Num() != Slots.Num())
	{
		BuildSlotWidgets();
	}

	// 각 슬롯 위젯에 슬롯 데이터 설정 및 선택 상태 업데이트
	for (int32 SlotIndex = 0; SlotIndex < Slots.Num() && SlotIndex < SlotWidgets.Num(); ++SlotIndex)
	{
		if (!SlotWidgets[SlotIndex])
		{
			continue;
		}

		SlotWidgets[SlotIndex]->RefreshSlot(Slots[SlotIndex]);
		SlotWidgets[SlotIndex]->SetSelected(SlotIndex == SelectedSlotIndex);
	}

	// 선택된 슬롯이 비었으면 정보 패널 초기화
	if (!InventoryComponent->IsValidSlotIndex(SelectedSlotIndex) || InventoryComponent->IsSlotEmpty(SelectedSlotIndex))
	{
		ClearSelectedItemInfo();
	}
}

void UTeloInventoryWidget::HandleSlotLeftClicked(int32 ClickedSlotIndex)
{
	ATeloPlayerCharacter* PlayerCharacter = Cast<ATeloPlayerCharacter>(GetOwningPlayerPawn());
	if (!PlayerCharacter)
	{
		return;
	}

	UTeloInventoryComponent* InventoryComponent = PlayerCharacter->GetInventoryComponent();
	if (!InventoryComponent)
	{
		return;
	}

	if (!InventoryComponent->IsValidSlotIndex(ClickedSlotIndex))
	{
		return;
	}

	const TArray<FTeloInventorySlot>& Slots = InventoryComponent->GetSlots();

	// 빈 슬롯을 클릭하면 선택 해제 및 정보 패널 초기화
	if (Slots[ClickedSlotIndex].IsEmpty())
	{
		SelectedSlotIndex = INDEX_NONE;
		ClearSelectedItemInfo();
		RefreshInventory();
		return;
	}

	// 아이템이 있는 슬롯이면 해당 슬롯을 선택 상태로 갱신
	SelectedSlotIndex = ClickedSlotIndex;
	SelectItem(Slots[ClickedSlotIndex].ItemData);
	RefreshInventory();
}

void UTeloInventoryWidget::HandleSlotDropped(int32 SourceSlotIndex, int32 TargetSlotIndex)
{
	ATeloPlayerCharacter* PlayerCharacter = Cast<ATeloPlayerCharacter>(GetOwningPlayerPawn());
	if (!PlayerCharacter)
	{
		return;
	}

	UTeloInventoryComponent* InventoryComponent = PlayerCharacter->GetInventoryComponent();
	if (!InventoryComponent)
	{
		return;
	}

	if (!InventoryComponent->IsValidSlotIndex(SourceSlotIndex) ||
		!InventoryComponent->IsValidSlotIndex(TargetSlotIndex))
	{
		return;
	}

	// 자기 자신에게 드롭한 경우는 이동 없이 UI만 갱신
	if (SourceSlotIndex == TargetSlotIndex)
	{
		RefreshInventory();
		return;
	}

	InventoryComponent->MoveSlot(SourceSlotIndex, TargetSlotIndex);

	// 드래그 이동 후 선택 상태와 정보 패널 초기화
	SelectedSlotIndex = INDEX_NONE;
	ClearSelectedItemInfo();
	RefreshInventory();
}

void UTeloInventoryWidget::SelectItem(const FTeloInventoryItem& ItemData)
{
	if (SelectedItemNameText)
	{
		SelectedItemNameText->SetText(ItemData.ItemName);
	}

	if (SelectedItemCountText)
	{
		SelectedItemCountText->SetText(FText::AsNumber(ItemData.Count));
	}

	if (SelectedItemDescriptionText)
	{
		SelectedItemDescriptionText->SetText(ItemData.Description);
	}
}

void UTeloInventoryWidget::ClearSelectedItemInfo()
{
	if (SelectedItemNameText)
	{
		SelectedItemNameText->SetText(FText::GetEmpty());
	}

	if (SelectedItemCountText)
	{
		SelectedItemCountText->SetText(FText::GetEmpty());
	}

	if (SelectedItemDescriptionText)
	{
		SelectedItemDescriptionText->SetText(FText::GetEmpty());
	}
}
