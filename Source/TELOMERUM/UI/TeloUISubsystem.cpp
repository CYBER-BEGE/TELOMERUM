// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TeloUISubsystem.h"

#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "Interfaces/TeloUIDataSource.h"
#include "UI/TeloScreenWidgetBase.h"
#include "UI/TeloInventoryWidget.h"
#include "UI/TeloTooltipWidget.h"
#include "UI/TeloContextWidget.h"
#include "Player/TeloPlayerCharacter.h"
#include "Player/TeloInteractComponent.h"

/* ==================== Subsystem Lifecycle ==================== */

void UTeloUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}


/* ==================== Screen Widget Management ==================== */

void UTeloUISubsystem::OpenScreenWidget(UTeloScreenWidgetBase* ScreenWidget)
{
	if (!ScreenWidget)
	{
		return;
	}

	// 다른 화면으로 전환하기 전에 현재 드래그 드롭 취소
	UWidgetBlueprintLibrary::CancelDragDrop();

	// 이미 열려있는 화면 위젯이 새로 열려고 하는 위젯과 다르다면, 현재 열려있는 화면 위젯 닫기
	if (CurrentScreenWidget && CurrentScreenWidget != ScreenWidget)
	{
		CurrentScreenWidget->OnScreenClosed();
		CurrentScreenWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	CurrentScreenWidget = ScreenWidget; // 입력받은 위젯을 현재 화면 위젯으로 설정

	HideTooltip(); // 툴팁 숨김
	HideContext(); // 아이템 컨텍스트 메뉴 숨김

	// 전체 화면 UI가 열리는 동안 상호작용 UI는 숨김
	if (ATeloPlayerCharacter* PlayerCharacter = Cast<ATeloPlayerCharacter>(GetLocalPlayer()->GetPlayerController(GetWorld())->GetPawn()))
	{
		if (UTeloInteractComponent* InteractComponent = PlayerCharacter->GetInteractComponent())
		{
			InteractComponent->SetInteractWidgetSuppressed(true);
		}
	}

	// 새로 열려고 하는 위젯 표시 및 UI 입력 모드 적용
	CurrentScreenWidget->SetVisibility(ESlateVisibility::Visible);
	CurrentScreenWidget->OnScreenOpened();

	ApplyUIInputMode(CurrentScreenWidget); // UI 입력 모드 적용
	CurrentScreenWidget->SetKeyboardFocus();
}

void UTeloUISubsystem::CloseScreenWidget(UTeloScreenWidgetBase* ScreenWidget)
{
	if (!ScreenWidget)
	{
		return;
	}

	// 현재 진행 중인 드래그 드롭이 있으면 먼저 취소
	UWidgetBlueprintLibrary::CancelDragDrop();

	ScreenWidget->OnScreenClosed();
	ScreenWidget->SetVisibility(ESlateVisibility::Hidden);

	HideTooltip();
	HideContext();

	// 현재 화면 위젯 초기화 및 게임 입력 모드 적용
	if (CurrentScreenWidget == ScreenWidget)
	{
		CurrentScreenWidget = nullptr;

		// 전체 화면 UI가 닫혔으니 상호작용 UI 다시 허용
		if (ATeloPlayerCharacter* PlayerCharacter = Cast<ATeloPlayerCharacter>(GetLocalPlayer()->GetPlayerController(GetWorld())->GetPawn()))
		{
			if (UTeloInteractComponent* InteractComponent = PlayerCharacter->GetInteractComponent())
			{
				InteractComponent->SetInteractWidgetSuppressed(false); // 상호작용 UI 다시 허용
			}
		}

		ApplyGameInputMode(); // 게임 입력 모드 적용
	}
}

bool UTeloUISubsystem::IsScreenWidgetOpen(const UTeloScreenWidgetBase* ScreenWidget) const
{
	return ScreenWidget													// 위젯 인스턴스가 존재하는지 확인
		&& ScreenWidget->IsInViewport()									// 위젯이 뷰포트에 있는지 확인
		&& ScreenWidget->GetVisibility() == ESlateVisibility::Visible;	// 위젯이 보이는 상태인지 확인
}


/* ==================== Input Mode Management ==================== */

void UTeloUISubsystem::ApplyUIInputMode(UTeloScreenWidgetBase* FocusWidget)
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

	// UI 입력 모드로 설정
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(FocusWidget ? FocusWidget->TakeWidget() : TSharedPtr<SWidget>()); // 위젯에 포커스 설정
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

	OpenScreenWidget(InventoryWidgetInstance); // 위젯을 열기
}

void UTeloUISubsystem::CloseInventory()
{
	if (!InventoryWidgetInstance)
	{
		return;
	}

	CloseScreenWidget(InventoryWidgetInstance); // 위젯을 닫기
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
	return IsScreenWidgetOpen(InventoryWidgetInstance); // 인벤토리 위젯이 열려있는지 여부 반환
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
	// 참조하는 객체가 없으면 컨텍스트 메뉴 숨김
	UObject* SourceObject = CurrentContextSource.Get();
	if (!SourceObject)
	{
		HideContext(); 
		return;
	}

	// 참조하는 객체가 ITeloUIDataSource 인터페이스를 구현하고 있다면, 액션 처리 함수 호출
	ITeloUIDataSource* Source = Cast<ITeloUIDataSource>(SourceObject);
	if (Source)
	{
		Source->HandleContextAction(ActionID);
	}

	// 액션 처리 후 컨텍스트 메뉴 숨김
	HideContext();

	// 컨텍스트 메뉴 액션 후 현재 화면 위젯에 입력 모드와 포커스를 다시 복구
	if (CurrentScreenWidget && CurrentScreenWidget->IsInViewport())
	{
		ApplyUIInputMode(CurrentScreenWidget);
		CurrentScreenWidget->SetKeyboardFocus();
	}
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
