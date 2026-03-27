// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TeloUIDataTypes.generated.h"

USTRUCT(BlueprintType)
struct FTeloTooltipData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tooltip")
	FText Title = FText::GetEmpty();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tooltip")
	FText Description = FText::GetEmpty();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tooltip")
	bool bUseDescription = false;
};

USTRUCT(BlueprintType)
struct FTeloContextAction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContextMenu")
	FName ActionID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContextMenu")
	FText Label = FText::GetEmpty();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContextMenu")
	bool bEnabled = true;
};

USTRUCT(BlueprintType)
struct FTeloContextData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContextMenu")
	TArray<FTeloContextAction> Actions;
};


class TELOMERUM_API TeloUIDataTypes
{
public:
	TeloUIDataTypes();
	~TeloUIDataTypes();
};
