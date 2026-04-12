// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/TeloGameModeBase.h"

#include "TeloStageGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TELOMERUM_API ATeloStageGameMode : public ATeloGameModeBase
{
	GENERATED_BODY()
	
protected:
	/* 스테이지 외곽 랜덤 스폰에 사용할 PlayerStart 태그 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
	FName StageSpawnTag = TEXT("StageSpawn");

protected:
	/* 스테이지용 PlayerStart 선택 */
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
};
