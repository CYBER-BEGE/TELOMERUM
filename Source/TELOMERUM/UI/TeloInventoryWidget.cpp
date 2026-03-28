// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TeloInventoryWidget.h"

#include "Engine/LocalPlayer.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Input/Reply.h"
#include "InputCoreTypes.h"

#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

#include "Player/TeloPlayerCharacter.h"
#include "Player/TeloInventoryComponent.h"
#include "UI/TeloUISubsystem.h"
#include "UI/TeloInventoryEntryWidget.h"

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

void UTeloInventoryWidget::RefreshInventory()
{
	if (!ItemWrapBox)
	{
		return;
	}

	ItemWrapBox->ClearChildren(); // 기존 아이템 목록 제거

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

	const TArray<FTeloInventoryItem>& Items = InventoryComponent->GetItems();
	if (Items.IsEmpty())
	{
		return;
	}

	if (!InventoryEntryWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UTeloInventoryWidget] InventoryEntryWidgetClass is NULL"));
		return;
	}

	for (const FTeloInventoryItem& Item : Items)
	{
		// 아이템 데이터를 표시할 위젯 생성
		UTeloInventoryEntryWidget* EntryWidget = CreateWidget<UTeloInventoryEntryWidget>(GetOwningPlayer(), InventoryEntryWidgetClass);
		if (!EntryWidget)
		{
			continue;
		}

		EntryWidget->SetItemData(Item); // 아이템 데이터를 위젯에 설정
		EntryWidget->OnInventoryEntryClicked.AddUObject(this, &UTeloInventoryWidget::SelectItem); // 아이템 클릭 시 SelectItem 함수 호출
		ItemWrapBox->AddChild(EntryWidget); // 스크롤 박스에 아이템 위젯 추가
	}
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
