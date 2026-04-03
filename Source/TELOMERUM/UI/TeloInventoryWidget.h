// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/TeloScreenWidgetBase.h"
#include "Components/UniformGridPanel.h"
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
	/* 화면이 닫힐 때 인벤토리 변경 델리게이트 바인딩 해제 */
	virtual void OnScreenClosed() override;
	
protected:
	/* 인벤토리 아이템을 담는 UniformGridPanel */
	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* SlotGridPanel;

	/* 인벤토리 아이템을 표시하는 위젯 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UTeloInventoryEntryWidget> InventoryEntryWidgetClass;

	/* 인벤토리 슬롯을 표시하는 위젯 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UTeloInventorySlotWidget> InventorySlotWidgetClass;

	/* 한 줄에 표시할 슬롯 개수 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true", ClampMin = "1"))
	int32 InventoryColumnCount = 5;

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

private:
	/* 현재 생성된 슬롯 위젯들 */
	UPROPERTY()
	TArray<class UTeloInventorySlotWidget*> SlotWidgets;

	/* 현재 선택된 슬롯 인덱스 */
	int32 SelectedSlotIndex = INDEX_NONE;

private:
	/* 슬롯 위젯들을 고정 개수로 생성 */
	void BuildSlotWidgets();

	/* 슬롯 좌클릭 처리 */
	void HandleSlotLeftClicked(int32 ClickedSlotIndex);

	/* 슬롯 드래그 드롭으로 아이템 이동이 발생했을 때 처리 */
	void HandleSlotDropped(int32 SourceSlotIndex, int32 TargetSlotIndex);

	/* 선택 아이템 정보 패널 초기화 */
	void ClearSelectedItemInfo();
};
