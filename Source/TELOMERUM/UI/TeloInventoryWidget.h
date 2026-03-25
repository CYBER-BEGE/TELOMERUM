// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "TeloInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class TELOMERUM_API UTeloInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/* 위젯 초기화 함수 */
	virtual bool Initialize() override;

	/* 키 입력 처리 함수 */
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	
	/* 인벤토리 UI를 새로고침하는 함수 */
	void RefreshInventory();

	/* 인벤토리 아이템이 선택됐을 때 호출되는 함수 */
	void SelectItem(const struct FTeloInventoryItem& ItemData);

protected:
	/* 인벤토리 아이템 목록을 표시하는 스크롤 박스 */
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ItemListScrollBox;

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
};
