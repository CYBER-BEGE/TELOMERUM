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
	void SetInteractText(const FText& NewText);

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* InteractTextBlock;
};
