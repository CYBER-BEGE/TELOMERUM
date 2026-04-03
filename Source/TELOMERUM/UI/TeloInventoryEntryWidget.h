// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player/TeloInventoryComponent.h"
#include "Interfaces/TeloUIDataSource.h"
#include "TeloInventoryEntryWidget.generated.h"

/**
 * 
 */
UCLASS()
class TELOMERUM_API UTeloInventoryEntryWidget : public UUserWidget, public ITeloUIDataSource
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryEntryClicked, const FTeloInventoryItem&);
	
public:
	virtual bool Initialize() override;

	/* 아이템 데이터를 위젯에 적용 */
	void SetItemData(const FTeloInventoryItem& ItemData);
	/* 현재 아이템이 들어있는 슬롯 인덱스를 설정 */
	void SetSlotIndex(int32 InSlotIndex);

	/* 아이템이 클릭됐을 때 부모 위젯에게 알리는 delegate */
	FOnInventoryEntryClicked OnInventoryEntryClicked;

	/* 툴팁 데이터를 가져옵 (반환값이 false인 경우 툴팁이 표시되지 않음) */
	virtual bool GetTooltipData(FTeloTooltipData& OutTooltipData) const override;
	/* 컨텍스트 메뉴 액션을 가져옵 */
	virtual void GetContextActions(TArray<FTeloContextAction>& OutActions) const override;
	/* 컨텍스트 메뉴 액션이 선택되었을 때 호출됨 */
	virtual void HandleContextAction(FName ActionID) override;

protected:
	/* 아이템 이름 텍스트 */
	UPROPERTY(meta = (BindWidget))
	class UImage* ItemIconImage;

	/* 아이템 개수 텍스트 (1개인 경우 숨김) */
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemCountText;

	/* 아이템 등급에 따른 테두리 이미지 */
	UPROPERTY(meta = (BindWidget))
	class UButton* EntryButton;

	/* 클릭 이벤트 핸들러 */
	UFUNCTION()
	void HandleEntryButtonClicked();

	/* 호버링 이벤트 핸들러 */
	UFUNCTION()
	void HandleEntryButtonHovered();

	/* 언호버링 이벤트 핸들러 */
	UFUNCTION()
	void HandleEntryButtonUnhovered();

private:
	/* 클릭됐을 때 어떤 아이템인지 부모에게 넘겨주기 위해 저장 */
	FTeloInventoryItem CachedItemData;

	/* 현재 아이템이 들어있는 슬롯 인덱스 */
	int32 CachedSlotIndex = INDEX_NONE;
};
