// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TeloInventoryEntryWidget.h"

#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"

#include "UI/TeloUISubsystem.h"
#include "Player/TeloPlayerCharacter.h"

bool UTeloInventoryEntryWidget::Initialize()
{
	const bool bResult = Super::Initialize();

	if (EntryButton)
	{
		// 클릭은 SlotWidget이 처리하고, EntryWidget은 호버 툴팁만 담당
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

void UTeloInventoryEntryWidget::SetSlotIndex(int32 InSlotIndex)
{
	CachedSlotIndex = InSlotIndex;
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
	// 사용 가능한 아이템일 때만 Use 액션 추가
	if (CachedItemData.bUsable)
	{
		FTeloContextAction UseAction;
		UseAction.ActionID = TEXT("Use");
		UseAction.Label = FText::FromString(TEXT("사용"));
		OutActions.Add(UseAction);
	}

	FTeloContextAction DropAction;
	DropAction.ActionID = TEXT("Drop");
	DropAction.Label = FText::FromString(TEXT("버리기"));
	OutActions.Add(DropAction);
}

void UTeloInventoryEntryWidget::HandleContextAction(FName ActionID)
{
	if (ActionID == TEXT("Use")) // Use 액션 선택 시
	{
		ATeloPlayerCharacter* PlayerCharacter = Cast<ATeloPlayerCharacter>(GetOwningPlayerPawn());
		if (!PlayerCharacter)
		{
			return;
		}

		const bool bUseSucceeded = PlayerCharacter->TryUseItemAtSlot(CachedSlotIndex); // 아이템 사용 시도

		UE_LOG(LogTemp, Log, TEXT("[UTeloInventoryEntryWidget] Use Item: %s / Result: %s"),
			*CachedItemData.ItemID.ToString(),
			bUseSucceeded ? TEXT("Success") : TEXT("Fail"));
	}
	else if (ActionID == TEXT("Drop")) // Drop 액션 선택 시
	{
		ATeloPlayerCharacter* PlayerCharacter = Cast<ATeloPlayerCharacter>(GetOwningPlayerPawn());
		if (!PlayerCharacter)
		{
			return;
		}

		const bool bDropSucceeded = PlayerCharacter->TryDropItemAtSlot(CachedSlotIndex); // 아이템 버리기 시도

		UE_LOG(LogTemp, Log, TEXT("[UTeloInventoryEntryWidget] Drop Item: %s / Result: %s"),
			*CachedItemData.ItemID.ToString(),
			bDropSucceeded ? TEXT("Success") : TEXT("Fail"));
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
