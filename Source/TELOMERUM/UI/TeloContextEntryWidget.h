// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/TeloUIDataTypes.h"
#include "TeloContextEntryWidget.generated.h"

/**
 * 
 */
UCLASS()
class TELOMERUM_API UTeloContextEntryWidget : public UUserWidget
{
	GENERATED_BODY()
	
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnContextEntryClicked, FName);

public:
	/* 위젯이 초기화될 때, 버튼 클릭 이벤트를 바인딩하는 함수 */
	virtual bool Initialize() override;

	/* 메뉴 항목의 데이터를 설정하는 함수 */
	void SetActionData(const FTeloContextAction& InActionData);

	/* 메뉴 항목이 클릭되었을 때, 해당 항목의 ActionID를 전달하는 델리게이트 */
	FOnContextEntryClicked OnEntryClicked;

protected:
	/* 메뉴 항목의 버튼 위젯에 대한 바인딩 변수 */
	UPROPERTY(meta = (BindWidget))
	class UButton* EntryButton;

	/* 메뉴 항목의 텍스트 위젯에 대한 바인딩 변수 */
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* EntryText;

private:
	/* 메뉴 항목의 데이터를 캐싱하는 변수 */
	FTeloContextAction CachedActionData;

	/* 메뉴 항목이 클릭되었을 때, 해당 항목의 ActionID를 전달하는 함수 */
	UFUNCTION()
	void HandleEntryButtonClicked();
};
