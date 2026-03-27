// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player/TeloInventoryComponent.h"
#include "TeloItemContextMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class TELOMERUM_API UTeloItemContextMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;

	void SetItemData(const FTeloInventoryItem& InItemData);

protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* UseButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* DropButton;

private:
	UPROPERTY()
	FTeloInventoryItem CachedItemData;

	UFUNCTION()
	void HandleUseButtonClicked();

	UFUNCTION()
	void HandleDropButtonClicked();
};
