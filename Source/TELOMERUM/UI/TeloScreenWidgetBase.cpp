// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TeloScreenWidgetBase.h"

#include "Engine/LocalPlayer.h"
#include "Input/Reply.h"
#include "InputCoreTypes.h"

#include "UI/TeloUISubsystem.h"

bool UTeloScreenWidgetBase::Initialize()
{
	const bool bResult = Super::Initialize();

	SetIsFocusable(true); // 위젯이 포커스를 받을 수 있도록 설정

	return bResult;
}

FReply UTeloScreenWidgetBase::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	const FKey PressedButton = InMouseEvent.GetEffectingButton();

	if (PressedButton == EKeys::LeftMouseButton || PressedButton == EKeys::RightMouseButton)	// 좌클릭, 우클릭일 때만 컨텍스트 바깥 클릭 검사
	{
		if (UTeloUISubsystem* UISubsystem = GetTeloUISubsystem())								// 로컬 플레이어의 UI 서브시스템을 가져옴
		{
			if (UISubsystem->IsContextOpen())													// 컨텍스트 메뉴가 열려 있을 때
			{
				const FVector2D ScreenPosition = InMouseEvent.GetScreenSpacePosition();

				if (!UISubsystem->IsScreenPositionInsideContext(ScreenPosition))				// 클릭한 위치가 컨텍스트 메뉴 바깥일 때
				{
					UISubsystem->HideContext();													// 컨텍스트 메뉴 숨김
				}
			}
		}
	}

	return Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
}

UTeloUISubsystem* UTeloScreenWidgetBase::GetTeloUISubsystem() const
{
	if (ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
	{
		return LocalPlayer->GetSubsystem<UTeloUISubsystem>(); // 로컬 플레이어의 UI 서브시스템을 반환
	}

	return nullptr;
}

void UTeloScreenWidgetBase::OnScreenOpened()
{
}

void UTeloScreenWidgetBase::OnScreenClosed()
{
}
