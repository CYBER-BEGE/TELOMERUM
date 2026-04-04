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
	
	/* ==================== Subsystem Lifecycle ==================== */
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/* ===================== UI Widget Properties ==================== */
private:
	/* 인벤토리 위젯 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UTeloInventoryWidget> InventoryWidgetClass;

	/* 툴팁 위젯 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UTeloTooltipWidget> TooltipWidgetClass;

	/* 아이템 컨텍스트 메뉴 위젯 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UTeloContextWidget> ContextWidgetClass;

	/* ===================== UI Widget Instances ==================== */
private:
	/* 인벤토리 위젯 인스턴스 */
	UPROPERTY()
	class UTeloInventoryWidget* InventoryWidgetInstance = nullptr;

	/* 툴팁 위젯 인스턴스 */
	UPROPERTY()
	class UTeloTooltipWidget* TooltipWidgetInstance = nullptr;

	/* 아이템 컨텍스트 메뉴 위젯 인스턴스 */
	UPROPERTY()
	class UTeloContextWidget* ContextWidgetInstance = nullptr;

	/* ===================== UI State ==================== */
private:
	/* 현재 화면에 표시된 UI 위젯 */
	UPROPERTY()
	class UTeloScreenWidgetBase* CurrentScreenWidget = nullptr;

	/* 현재 아이템 컨텍스트 메뉴가 참조하는 객체 (예: 아이템 데이터) */
	TWeakObjectPtr<UObject> CurrentContextSource;

	/* ==================== Screen Widget Setup ==================== */
public:
	/* 위젯 열기 (다른 위젯이 이미 열려있다면 닫고 새로 열기) */
	void OpenScreenWidget(class UTeloScreenWidgetBase* ScreenWidget);
	/* 위젯 닫고 게임 입력 모드로 돌아가기 */
	void CloseScreenWidget(class UTeloScreenWidgetBase* ScreenWidget);
	/* ScreenWidget이 현재 열려있는지 확인 */
	bool IsScreenWidgetOpen(const class UTeloScreenWidgetBase* ScreenWidget) const;

	/* ==================== Input Mode Management ==================== */
private:
	/* UI 입력 모드 적용 */
	void ApplyUIInputMode(class UTeloScreenWidgetBase* FocusWidget);
	/* 게임 입력 모드 적용 */
	void ApplyGameInputMode();

	/* ==================== Inventory ==================== */
public:
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
private:
	/* 인벤토리 위젯 생성 */
	void CreateInventoryWidget();

	/* ==================== Tooltip ===================== */
public:
	/* 툴팁 위젯 클래스 설정 */
	void SetTooltipWidgetClass(TSubclassOf<class UTeloTooltipWidget> InWidgetClass);
	/* 툴팁 표시 (TooltipData를 직접 표시할 때 사용) */
	void ShowTooltipFromData(const FTeloTooltipData& InTooltipData);
	/* 툴팁 표시 (SourceObject가 ITeloUIDataSource 인터페이스를 구현하고 있을 때 사용) */
	void ShowTooltipFromSource(UObject* SourceObject);
	/* 툴팁 숨김 */
	void HideTooltip();
private:
	/* 툴팁 위젯 생성 */
	void CreateTooltipWidget();

	/* ==================== Item Context Menu ==================== */
public:
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
	/* 아이템 컨텍스트 메뉴 위젯 생성 */
	void CreateContextWidget();
	/* 아이템 컨텍스트 메뉴 액션 클릭 처리 */
	void HandleContextActionClicked(FName ActionID);
};
