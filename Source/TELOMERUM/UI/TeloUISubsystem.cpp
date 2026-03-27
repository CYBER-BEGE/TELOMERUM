// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TeloUISubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/TeloInventoryWidget.h"
#include "UI/TeloItemContextMenuWidget.h"

void UTeloUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}


/* ==================== Inventory ==================== */

void UTeloUISubsystem::SetInventoryWidgetClass(TSubclassOf<UTeloInventoryWidget> InWidgetClass)
{
	InventoryWidgetClass = InWidgetClass; // 위젯 클래스 설정
	CreateInventoryWidget();
}

void UTeloUISubsystem::OpenInventory()
{
	if (!InventoryWidgetInstance) // 위젯 인스턴스가 없으면 생성
	{
		CreateInventoryWidget();
	}

	if (!InventoryWidgetInstance)
	{
		return;
	}

	InventoryWidgetInstance->SetVisibility(ESlateVisibility::Visible); // 위젯 표시
	InventoryWidgetInstance->RefreshInventory(); // 인벤토리 UI 새로고침
	ApplyUIInputMode(); // UI 입력 모드 적용
	InventoryWidgetInstance->SetKeyboardFocus(); // 위젯에 키보드 포커스 설정
}

void UTeloUISubsystem::CloseInventory()
{
	if (!InventoryWidgetInstance)
	{
		return;
	}

	InventoryWidgetInstance->SetVisibility(ESlateVisibility::Hidden); // 위젯 숨김
	ApplyGameInputMode();	// 게임 입력 모드 적용

	HideTooltip();			// 인벤토리 닫을 때 툴팁도 숨김
	HideItemContextMenu();	// 인벤토리 닫을 때 아이템 컨텍스트 메뉴도 숨김
}

void UTeloUISubsystem::ToggleInventory()
{
	if (IsInventoryOpen())
	{
		CloseInventory();
	}
	else
	{
		OpenInventory();
	}
}

bool UTeloUISubsystem::IsInventoryOpen() const
{
	return InventoryWidgetInstance													// 위젯 인스턴스가 존재하는지 확인
		&& InventoryWidgetInstance->IsInViewport()									// 위젯이 뷰포트에 있는지 확인
		&& InventoryWidgetInstance->GetVisibility() == ESlateVisibility::Visible;	// 위젯이 보이는 상태인지 확인
}

void UTeloUISubsystem::CreateInventoryWidget()
{
	if (!InventoryWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UTeloUISubsystem] InventoryWidgetClass is NULL"));
		return;
	}

	if (InventoryWidgetInstance) // 위젯 중복 생성 방지
	{
		return;
	}

	ULocalPlayer* LocalPlayer = GetLocalPlayer(); // 로컬 플레이어 가져오기
	if (!LocalPlayer)
	{
		return;
	}

	APlayerController* PlayerController = LocalPlayer->GetPlayerController(GetWorld());
	if (!PlayerController)
	{
		return;
	}

	InventoryWidgetInstance = CreateWidget<UTeloInventoryWidget>(PlayerController, InventoryWidgetClass);
	if (InventoryWidgetInstance)
	{
		InventoryWidgetInstance->AddToViewport();
		InventoryWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UTeloUISubsystem::ApplyUIInputMode()
{
	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (!LocalPlayer)
	{
		return;
	}

	APlayerController* PlayerController = LocalPlayer->GetPlayerController(GetWorld());
	if (!PlayerController)
	{
		return;
	}

	// UI 입력 모드 설정
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(InventoryWidgetInstance ? InventoryWidgetInstance->TakeWidget() : TSharedPtr<SWidget>()); // 인벤토리 위젯에 포커스 설정
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // 마우스가 뷰포트에 고정되지 않도록 설정

	PlayerController->SetInputMode(InputMode); // 입력 모드 적용
	PlayerController->bShowMouseCursor = true; // 마우스 커서 표시
}

void UTeloUISubsystem::ApplyGameInputMode()
{
	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (!LocalPlayer)
	{
		return;
	}

	APlayerController* PlayerController = LocalPlayer->GetPlayerController(GetWorld());
	if (!PlayerController)
	{
		return;
	}

	FInputModeGameOnly InputMode;
	PlayerController->SetInputMode(InputMode); // 게임 입력 모드 적용
	PlayerController->bShowMouseCursor = false; // 마우스 커서 숨김
}


/* ==================== Tooltip ===================== */

void UTeloUISubsystem::SetTooltipWidgetClass(TSubclassOf<UTeloTooltipWidget> InWidgetClass)
{
	TooltipWidgetClass = InWidgetClass;
	CreateTooltipWidget();
}

void UTeloUISubsystem::CreateTooltipWidget()
{
	if (!TooltipWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UTeloUISubsystem] TooltipWidgetClass is NULL"));
		return;
	}

	if (TooltipWidgetInstance)
	{
		return;
	}

	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (!LocalPlayer)
	{
		return;
	}

	APlayerController* PlayerController = LocalPlayer->GetPlayerController(GetWorld());
	if (!PlayerController)
	{
		return;
	}

	TooltipWidgetInstance = CreateWidget<UTeloTooltipWidget>(PlayerController, TooltipWidgetClass);
	if (TooltipWidgetInstance)
	{
		TooltipWidgetInstance->AddToViewport(100); // ZOrder를 100으로 설정하여 다른 UI 요소보다 위에 표시
		TooltipWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UTeloUISubsystem::ShowTooltip(const FTeloTooltipData& InTooltipData)
{
	if (!TooltipWidgetInstance)
	{
		CreateTooltipWidget();
	}

	if (!TooltipWidgetInstance)
	{
		return;
	}

	TooltipWidgetInstance->SetTooltipData(InTooltipData);
	TooltipWidgetInstance->SetVisibility(ESlateVisibility::Visible);
}

void UTeloUISubsystem::HideTooltip()
{
	if (!TooltipWidgetInstance)
	{
		return;
	}

	TooltipWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
}


/* ==================== Item Context Menu ==================== */

void UTeloUISubsystem::SetItemContextMenuWidgetClass(TSubclassOf<UTeloItemContextMenuWidget> InWidgetClass)
{
	ItemContextMenuWidgetClass = InWidgetClass;
	CreateItemContextMenuWidget();
}

void UTeloUISubsystem::CreateItemContextMenuWidget()
{
	if (!ItemContextMenuWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UTeloUISubsystem] ItemContextMenuWidgetClass is NULL"));
		return;
	}

	if (ItemContextMenuWidgetInstance)
	{
		return;
	}

	ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (!LocalPlayer)
	{
		return;
	}

	APlayerController* PlayerController = LocalPlayer->GetPlayerController(GetWorld());
	if (!PlayerController)
	{
		return;
	}

	ItemContextMenuWidgetInstance = CreateWidget<UTeloItemContextMenuWidget>(PlayerController, ItemContextMenuWidgetClass);
	if (ItemContextMenuWidgetInstance)
	{
		ItemContextMenuWidgetInstance->AddToViewport(110);
		ItemContextMenuWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UTeloUISubsystem::ShowItemContextMenu(const FTeloInventoryItem& InItemData, const FVector2D& ScreenPosition)
{
	if (!ItemContextMenuWidgetInstance)
	{
		CreateItemContextMenuWidget();
	}

	if (!ItemContextMenuWidgetInstance)
	{
		return;
	}

	ItemContextMenuWidgetInstance->SetItemData(InItemData);
	ItemContextMenuWidgetInstance->SetPositionInViewport(ScreenPosition + FVector2D(8.0f, 8.0f), true);
	ItemContextMenuWidgetInstance->SetVisibility(ESlateVisibility::Visible);
}

void UTeloUISubsystem::HideItemContextMenu()
{
	if (!ItemContextMenuWidgetInstance)
	{
		return;
	}

	ItemContextMenuWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
}