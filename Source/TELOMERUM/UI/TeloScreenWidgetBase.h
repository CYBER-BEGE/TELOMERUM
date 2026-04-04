// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "TeloScreenWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class TELOMERUM_API UTeloScreenWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/* 위젯 초기화 함수 */
	virtual bool Initialize() override;
	/* 키 입력 처리 함수 (마우스 버튼 입력을 자식 위젯보다 먼저 처리) */
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

protected:
	/* 화면에 표시된 UI가 컨텍스트 메뉴 바깥에서 클릭됐을 때 호출되는 함수 */
	class UTeloUISubsystem* GetTeloUISubsystem() const;

public:
	/* 화면이 열릴 때 호출되는 공통 훅 */
	virtual void OnScreenOpened();
	/* 화면이 닫힐 때 호출되는 공통 훅 */
	virtual void OnScreenClosed();
};
