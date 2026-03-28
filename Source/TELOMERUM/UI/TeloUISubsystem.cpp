// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TeloUISubsystem.h"

#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"

#include "UI/TeloInventoryWidget.h"
#include "UI/TeloTooltipWidget.h"
#include "UI/TeloContextWidget.h"
#include "Interfaces/TeloUIDataSource.h"

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
	HideContext();			// 인벤토리 닫을 때 아이템 컨텍스트 메뉴도 숨김
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
	if (!InventoryWidgetClass || InventoryWidgetInstance) // 클래스가 없거나, 위젯이 이미 있다면
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
	PlayerController->SetInputMode(InputMode);	// 게임 입력 모드 적용
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
	if (!TooltipWidgetClass || TooltipWidgetInstance) // 클래스가 없거나, 위젯이 이미 있다면
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

void UTeloUISubsystem::ShowTooltipFromData(const FTeloTooltipData& InTooltipData)
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

void UTeloUISubsystem::ShowTooltipFromSource(UObject* SourceObject)
{
	if (!SourceObject)
	{
		return;
	}

	ITeloUIDataSource* Source = Cast<ITeloUIDataSource>(SourceObject);
	if (!Source)
	{
		return;
	}

	FTeloTooltipData TooltipData;
	if (!Source->GetTooltipData(TooltipData))
	{
		HideTooltip();
		return;
	}

	ShowTooltipFromData(TooltipData);
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

void UTeloUISubsystem::SetContextWidgetClass(TSubclassOf<UTeloContextWidget> InWidgetClass)
{
	ContextWidgetClass = InWidgetClass;
	CreateContextWidget();
}

void UTeloUISubsystem::CreateContextWidget()
{
	if (!ContextWidgetClass || ContextWidgetInstance)
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

	ContextWidgetInstance = CreateWidget<UTeloContextWidget>(PlayerController, ContextWidgetClass);
	if (ContextWidgetInstance)
	{
		ContextWidgetInstance->AddToViewport(110); // ZOrder를 110으로 설정하여 툴팁보다 위에 표시
		ContextWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		ContextWidgetInstance->OnActionClicked.AddUObject(this, &UTeloUISubsystem::HandleContextActionClicked);
	}
}

void UTeloUISubsystem::ShowContextFromData(const FTeloContextData& InMenuData, UObject* SourceObject, const FVector2D& ScreenPosition)
{
	if (!ContextWidgetInstance)
	{
		CreateContextWidget();
	}

	if (!ContextWidgetInstance)
	{
		return;
	}

	CurrentContextSource = SourceObject;

	ContextWidgetInstance->SetMenuData(InMenuData);
	ContextWidgetInstance->SetPositionInViewport(ScreenPosition + FVector2D(8.0f, 8.0f), true);
	ContextWidgetInstance->SetVisibility(ESlateVisibility::Visible);
}

void UTeloUISubsystem::ShowContextFromSource(UObject* SourceObject, const FVector2D& ScreenPosition)
{
	if (!SourceObject)
	{
		return;
	}

	ITeloUIDataSource* Source = Cast<ITeloUIDataSource>(SourceObject);
	if (!Source)
	{
		return;
	}

	FTeloContextData MenuData;
	Source->GetContextActions(MenuData.Actions);

	if (MenuData.Actions.IsEmpty())
	{
		HideContext();
		return;
	}

	ShowContextFromData(MenuData, SourceObject, ScreenPosition);
}

void UTeloUISubsystem::HideContext()
{
	CurrentContextSource = nullptr;

	if (!ContextWidgetInstance)
	{
		return;
	}

	ContextWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
}

void UTeloUISubsystem::HandleContextActionClicked(FName ActionID)
{
	UObject* SourceObject = CurrentContextSource.Get();
	if (!SourceObject)
	{
		HideContext();
		return;
	}

	ITeloUIDataSource* Source = Cast<ITeloUIDataSource>(SourceObject);
	if (Source)
	{
		Source->HandleContextAction(ActionID);
	}

	HideContext();
}

bool UTeloUISubsystem::IsContextOpen() const
{
	return ContextWidgetInstance
		&& ContextWidgetInstance->IsInViewport()
		&& ContextWidgetInstance->GetVisibility() == ESlateVisibility::Visible;
}

bool UTeloUISubsystem::IsScreenPositionInsideContext(const FVector2D& ScreenPosition) const
{
	if (!ContextWidgetInstance)
	{
		return false;
	}

	return ContextWidgetInstance->IsScreenPositionInside(ScreenPosition);
}
