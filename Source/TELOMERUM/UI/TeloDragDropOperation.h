// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "Player/TeloInventoryComponent.h"
#include "TeloDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class TELOMERUM_API UTeloDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	/* 드래그가 시작된 원본 슬롯 인덱스 */
	UPROPERTY(BlueprintReadWrite)
	int32 SourceSlotIndex = INDEX_NONE;

	/* 드래그 중인 아이템 데이터 */
	UPROPERTY(BlueprintReadWrite)
	FTeloInventoryItem DraggedItemData;
};
