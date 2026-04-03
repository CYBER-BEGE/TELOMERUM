// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "UI/TeloUIDataTypes.h"
#include "TeloUISubsystem.generated.h"

/**
 * 
 */
UCLASS()
class TELOMERUM_API UTeloUISubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	/* 인벤토리 위젯 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UTeloInventoryWidget> InventoryWidgetClass;

	/* 인벤토리 위젯 인스턴스 */
	UPROPERTY()
	class UTeloInventoryWidget* InventoryWidgetInstance = nullptr;

	/* 툴팁 위젯 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UTeloTooltipWidget> TooltipWidgetClass;

	/* 툴팁 위젯 인스턴스 */
	UPROPERTY()
	class UTeloTooltipWidget* TooltipWidgetInstance = nullptr;

	/* 아이템 컨텍스트 메뉴 위젯 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UTeloContextWidget> ContextWidgetClass;

	/* 아이템 컨텍스트 메뉴 위젯 인스턴스 */
	UPROPERTY()
	class UTeloContextWidget* ContextWidgetInstance = nullptr;

	/* 현재 아이템 컨텍스트 메뉴가 참조하는 객체 (예: 아이템 데이터) */
	TWeakObjectPtr<UObject> CurrentContextSource;

	/* 현재 화면에 표시된 UI 위젯 */
	UPROPERTY()
	class UTeloScreenWidgetBase* CurrentScreenWidget = nullptr;

public:
	/* 위젯 열기 (다른 위젯이 이미 열려있다면 닫고 새로 열기) */
	void OpenScreenWidget(class UTeloScreenWidgetBase* ScreenWidget);
	/* 위젯 닫고 게임 입력 모드로 돌아가기 */
	void CloseScreenWidget(class UTeloScreenWidgetBase* ScreenWidget);
	/* ScreenWidget이 현재 열려있는지 확인 */
	bool IsScreenWidgetOpen(const class UTeloScreenWidgetBase* ScreenWidget) const;

	/* 인벤토리 위젯 클래스 설정 */
	void SetInventoryWidgetClass(TSubclassOf<class UTeloInventoryWidget> InWidgetClass);
	/* 인벤토리 위젯 표시 */
	void OpenInventory();
	/* 인벤토리 위젯 숨김 */
	void CloseInventory();
	/* 인벤토리 위젯 토글 */
	void ToggleInventory();
	/* 인벤토리 위젯이 열려있는지 여부 반환 */
	bool IsInventoryOpen() const;

	/* 툴팁 위젯 클래스 설정 */
	void SetTooltipWidgetClass(TSubclassOf<class UTeloTooltipWidget> InWidgetClass);
	/* 툴팁 표시 (TooltipData를 직접 표시할 때 사용) */
	void ShowTooltipFromData(const FTeloTooltipData& InTooltipData);
	/* 툴팁 표시 (SourceObject가 ITeloUIDataSource 인터페이스를 구현하고 있을 때 사용) */
	void ShowTooltipFromSource(UObject* SourceObject);
	/* 툴팁 숨김 */
	void HideTooltip();

	/* 아이템 컨텍스트 메뉴 위젯 클래스 설정 */
	void SetContextWidgetClass(TSubclassOf<class UTeloContextWidget> InWidgetClass);
	/* 아이템 컨텍스트 메뉴 표시 (TooltipData를 직접 표시할 때 사용) */
	void ShowContextFromData(const FTeloContextData& InMenuData, UObject* SourceObject, const FVector2D& ScreenPosition);
	/* 아이템 컨텍스트 메뉴 표시 (SourceObject가 ITeloUIDataSource 인터페이스를 구현하고 있을 때 사용) */
	void ShowContextFromSource(UObject* SourceObject, const FVector2D& ScreenPosition);
	/* 아이템 컨텍스트 메뉴 숨김 */
	void HideContext();

	/* 컨텍스트 메뉴가 현재 열려 있는지 확인 */
	bool IsContextOpen() const;
	/* 화면 좌표가 현재 열려 있는 컨텍스트 메뉴 안인지 확인 */
	bool IsScreenPositionInsideContext(const FVector2D& ScreenPosition) const;

private:
	/* 인벤토리 위젯 생성 */
	void CreateInventoryWidget();
	/* 툴팁 위젯 생성 */
	void CreateTooltipWidget();
	/* 아이템 컨텍스트 메뉴 위젯 생성 */
	void CreateContextWidget();

	/* 아이템 컨텍스트 메뉴 액션 클릭 처리 */
	void HandleContextActionClicked(FName ActionID);

	/* UI 입력 모드 적용 */
	void ApplyUIInputMode(class UTeloScreenWidgetBase* FocusWidget);
	/* 게임 입력 모드 적용 */
	void ApplyGameInputMode();
};
