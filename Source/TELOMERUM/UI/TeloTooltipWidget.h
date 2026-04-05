// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "UI/TeloUIDataTypes.h"

#include "TeloTooltipWidget.generated.h"

UCLASS()
class TELOMERUM_API UTeloTooltipWidget : public UUserWidget
{
	GENERATED_BODY()
	

	/* ==================== UserWidget Overrides ==================== */
public:
	/* 툴팁이 visible일 때 자기 스스로 마우스 위치를 따라가게 함 */
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


	/* ==================== Tooltip Properties ==================== */
protected:
	/* 마우스 커서로부터 툴팁 위치 오프셋 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tooltip", meta = (AllowPrivateAccess = "true"))
	FVector2D CursorOffset = FVector2D(12.0f, 12.0f);


	/* ===================== UI Widget Components ==================== */
protected:
	/* 툴팁 제목 텍스트 블록 */
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TitleText;

	/* 툴팁 설명 텍스트 블록 */
	UPROPERTY(meta = (BindWidgetOptional))
	class UTextBlock* DescriptionText;


	/* ===================== Tooltip Setup ==================== */
public:
	/* 툴팁 데이터로 UI 업데이트 */
	void SetTooltipData(const FTeloTooltipData& InTooltipData);


	/* ===================== Internal Functions ==================== */
private:
	/* 마우스 위치에 따라 툴팁 위치 업데이트 */
	void UpdateTooltipPosition();
};
