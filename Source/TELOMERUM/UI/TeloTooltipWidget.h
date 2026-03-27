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
	
public:
	/* 툴팁이 visible일 때 자기 스스로 마우스 위치를 따라가게 함 */
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetTooltipData(const FTeloTooltipData& InTooltipData);

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TitleText;

	UPROPERTY(meta = (BindWidgetOptional))
	class UTextBlock* DescriptionText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tooltip", meta = (AllowPrivateAccess = "true"))
	FVector2D CursorOffset = FVector2D(12.0f, 12.0f);

private:
	void UpdateTooltipPosition();
};
