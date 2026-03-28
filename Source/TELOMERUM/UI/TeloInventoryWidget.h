// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/TeloScreenWidgetBase.h"
#include "Components/WrapBox.h"
#include "TeloInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class TELOMERUM_API UTeloInventoryWidget : public UTeloScreenWidgetBase
{
	GENERATED_BODY()

public:
	/* 키 입력 처리 함수 */
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	/* 화면이 열릴 때 호출되는 함수 */
	virtual void OnScreenOpened() override;
	
protected:
	/* 인벤토리 아이템 목록을 표시하는 균등 그리드 패널 */
	UPROPERTY(meta = (BindWidget))
	class UWrapBox* ItemWrapBox;

	/* 인벤토리 아이템을 표시하는 위젯 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UTeloInventoryEntryWidget> InventoryEntryWidgetClass;

	/* 선택된 아이템의 이름을 표시하는 텍스트 블록 */
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SelectedItemNameText;

	/* 선택된 아이템의 개수를 표시하는 텍스트 블록 */
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SelectedItemCountText;

	/* 선택된 아이템의 설명을 표시하는 텍스트 블록 */
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SelectedItemDescriptionText;

public:
	/* 인벤토리 UI를 새로고침하는 함수 */
	void RefreshInventory();
	/* 인벤토리 아이템이 선택됐을 때 호출되는 함수 */
	void SelectItem(const struct FTeloInventoryItem& ItemData);
};
