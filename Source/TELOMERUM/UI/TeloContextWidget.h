// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "UI/TeloUIDataTypes.h"

#include "TeloContextWidget.generated.h"

/**
 * 
 */
UCLASS()
class TELOMERUM_API UTeloContextWidget : public UUserWidget
{
	GENERATED_BODY()
	
	/* 컨텍스트 메뉴가 클릭되었을 때, 해당 항목의 ActionID를 전달하는 델리게이트 선언 */
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnContextActionClicked, FName);


	/* ==================== Widget Components ==================== */
protected:
	/* 메뉴 항목들을 담는 VerticalBox 위젯에 대한 바인딩 변수 */
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* ActionListBox;

	/* 메뉴 항목으로 사용할 위젯 클래스에 대한 변수, 블루프린트에서 설정 가능 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ContextMenu")
	TSubclassOf<class UTeloContextEntryWidget> ContextEntryWidgetClass;


	/* ==================== Delegates ==================== */
public:
	/* 메뉴 항목이 클릭되었을 때, 해당 항목의 ActionID를 전달하는 델리게이트 */
	FOnContextActionClicked OnActionClicked;


	/* ==================== Context Menu Setup ==================== */
public:
	/* 메뉴 데이터를 설정하는 함수 */
	void SetMenuData(const FTeloContextData& InMenuData);
	/* 화면 좌표가 현재 컨텍스트 위젯 영역 안에 있는지 검사 */
	bool IsScreenPositionInside(const FVector2D& ScreenPosition) const;


	/* ==================== Internal Functions ==================== */
private:
	/* 메뉴 항목을 모두 제거하는 함수 */
	void ClearEntries();
	/* 메뉴 데이터를 기반으로 새로운 메뉴 항목을 추가하는 함수 */
	void AddEntry(const FTeloContextAction& InActionData);
	/* 메뉴 항목이 클릭되었을 때, 해당 항목의 ActionID를 전달하는 함수 */
	void HandleEntryClicked(FName ActionID);
};
