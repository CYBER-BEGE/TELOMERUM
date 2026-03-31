// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloPlayerController.h"

#include "EnhancedInputSubsystems.h"

#include "UI/TeloUISubsystem.h"
#include "UI/TeloInventoryWidget.h"
#include "UI/TeloTooltipWidget.h"
#include "UI/TeloContextWidget.h"

ATeloPlayerController::ATeloPlayerController()
{

}

void ATeloPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (DefaultMappingContexts.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("[ATeloPlayerController] DefaultMappingContexts is NULL"));
	}
	if (!InventoryWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ATeloPlayerController] InventoryWidgetClass is NULL"));
	}
	if (!TooltipWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ATeloPlayerController] TooltipWidgetClass is NULL"));
	}
	if (!ContextWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ATeloPlayerController] ContextWidgetClass is NULL"));
	}

	// UI 서브시스템에 위젯 클래스 설정
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UTeloUISubsystem* UISubsystem = LocalPlayer->GetSubsystem<UTeloUISubsystem>())
		{
			UISubsystem->SetInventoryWidgetClass(InventoryWidgetClass);
			UISubsystem->SetTooltipWidgetClass(TooltipWidgetClass);
			UISubsystem->SetContextWidgetClass(ContextWidgetClass);
		}
	}
}

void ATeloPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// 로컬 플레이어 컨트롤러용 IMC만 추가
	if (IsLocalPlayerController())
	{
		// 입력 매핑 컨텍스트 추가
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}
		}
	}
}