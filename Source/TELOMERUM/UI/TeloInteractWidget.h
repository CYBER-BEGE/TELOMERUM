// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "TeloInteractWidget.generated.h"

/**
 * 
 */
UCLASS()
class TELOMERUM_API UTeloInteractWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/* 인터랙션 텍스트 설정 */
	void SetInteractText(const FText& NewText);

protected:
	/* 인터랙션 텍스트 블록 */
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* InteractTextBlock;
};
