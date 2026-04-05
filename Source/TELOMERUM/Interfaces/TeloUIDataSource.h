// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "UI/TeloUIDataTypes.h"

#include "TeloUIDataSource.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTeloUIDataSource : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TELOMERUM_API ITeloUIDataSource
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/* 툴팁 데이터를 가져옵 (반환값이 false인 경우 툴팁이 표시되지 않음) */
	virtual bool GetTooltipData(FTeloTooltipData& OutTooltipData) const
	{
		return false;
	}

	/* 컨텍스트 메뉴 액션을 가져옵 */
	virtual void GetContextActions(TArray<FTeloContextAction>& OutActions) const
	{}

	/* 컨텍스트 메뉴 액션이 선택되었을 때 호출됨 */
	virtual void HandleContextAction(FName ActionID)
	{}
};
