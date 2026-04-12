// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/TeloVillGameMode.h"

#include "GameFramework/PlayerStart.h"

AActor* ATeloVillGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	if (APlayerStart* TaggedStart = GetRandomPlayerStartByTag(VillSpawnTag))
	{
		return TaggedStart;
	}

	return Super::ChoosePlayerStart_Implementation(Player);
}
