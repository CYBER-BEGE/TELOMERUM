// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/TeloGameModeBase.h"

#include "TeloVillGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TELOMERUM_API ATeloVillGameMode : public ATeloGameModeBase
{
	GENERATED_BODY()
	
protected:
	/* 마을에서 사용할 기본 스폰 태그 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
	FName VillSpawnTag = TEXT("VillSpawn");

protected:
	/* 마을용 PlayerStart 선택 */
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
};
