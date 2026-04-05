// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TeloUIDataTypes.generated.h"

USTRUCT(BlueprintType)
struct FTeloTooltipData
{
	GENERATED_BODY()

	/* 툴팁 제목 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tooltip")
	FText Title = FText::GetEmpty();

	/* 툴팁 설명 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tooltip")
	FText Description = FText::GetEmpty();

	/* 툴팁 설명 사용 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tooltip")
	bool bUseDescription = false;
};

USTRUCT(BlueprintType)
struct FTeloContextAction
{
	GENERATED_BODY()

	/* 컨텍스트 메뉴 버튼 액션 ID */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContextMenu")
	FName ActionID = NAME_None;

	/* 컨텍스트 메뉴 버튼 라벨 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContextMenu")
	FText Label = FText::GetEmpty();

	/* 컨텍스트 메뉴 버튼 활성화 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContextMenu")
	bool bEnabled = true;
};

USTRUCT(BlueprintType)
struct FTeloContextData
{
	GENERATED_BODY()

	/* 컨텍스트 메뉴 액션 리스트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ContextMenu")
	TArray<FTeloContextAction> Actions;
};


class TELOMERUM_API TeloUIDataTypes
{

};
