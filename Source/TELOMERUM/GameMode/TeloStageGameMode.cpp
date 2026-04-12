// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/TeloStageGameMode.h"

#include "GameFramework/PlayerStart.h"

AActor* ATeloStageGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	if (APlayerStart* TaggedStart = GetRandomPlayerStartByTag(StageSpawnTag))
	{
		return TaggedStart;
	}

	return Super::ChoosePlayerStart_Implementation(Player);
}
