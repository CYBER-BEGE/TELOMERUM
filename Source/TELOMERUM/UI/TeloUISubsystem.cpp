// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TeloUISubsystem.h"
#include "UI/TeloInventoryWidget.h"
#include "Blueprint/UserWidget.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UTeloUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

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
	ApplyUIInputMode(); // UI 입력 모드 적용
}

void UTeloUISubsystem::CloseInventory()
{
	if (!InventoryWidgetInstance)
	{
		return;
	}

	InventoryWidgetInstance->SetVisibility(ESlateVisibility::Hidden); // 위젯 숨김
	ApplyGameInputMode(); // 게임 입력 모드 적용
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