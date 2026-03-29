// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player/TeloInventoryComponent.h"
#include "TeloInventorySlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class TELOMERUM_API UTeloInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventorySlotLeftClicked, int32);

public:
	/* 슬롯 마우스 입력을 자식 위젯보다 먼저 처리 */
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	/* 슬롯 인덱스 설정 */
	void SetSlotIndex(int32 InSlotIndex);

	/* 슬롯 안에 표시할 아이템 위젯 클래스 설정 */
	void SetEntryWidgetClass(TSubclassOf<class UTeloInventoryEntryWidget> InEntryWidgetClass);

	/* 슬롯 데이터 기준으로 UI 갱신 */
	void RefreshSlot(const FTeloInventorySlot& InSlotData);

	/* 현재 슬롯의 선택 상태 표시 */
	void SetSelected(bool bInSelected);

	/* 좌클릭 시 슬롯 인덱스를 알리는 델리게이트 */
	FOnInventorySlotLeftClicked OnSlotLeftClicked;

private:
	/* 로컬 플레이어의 UI 서브시스템 반환 */
	class UTeloUISubsystem* GetTeloUISubsystem() const;

protected:
	/* 슬롯 외곽선 */
	UPROPERTY(meta = (BindWidgetOptional))
	class UBorder* SlotBorder;

	/* 슬롯 내부에 아이템 위젯을 담는 컨테이너 */
	UPROPERTY(meta = (BindWidget))
	class UOverlay* ItemContainer;

private:
	/* 현재 슬롯 인덱스 */
	int32 CachedSlotIndex = INDEX_NONE;

	/* 현재 슬롯 데이터 */
	FTeloInventorySlot CachedSlotData;

	/* 슬롯 내부에 생성된 아이템 위젯 */
	UPROPERTY()
	class UTeloInventoryEntryWidget* EntryWidgetInstance = nullptr;

	/* 슬롯 내부에서 사용할 아이템 위젯 클래스 */
	UPROPERTY()
	TSubclassOf<class UTeloInventoryEntryWidget> InventoryEntryWidgetClass;
};
