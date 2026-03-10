// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloPlayerController.h"
#include "EnhancedInputSubsystems.h"

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

	///* HUD 위젯 생성 및 추가 */
	//HUD = CreateWidget(this, HUDClass);
	//if (HUD != nullptr)
	//{
	//	HUD->AddToViewport();
	//}
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