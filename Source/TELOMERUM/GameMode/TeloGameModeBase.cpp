// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloGameModeBase.h"

#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"


/* ============================== Random PlayerStart ============================== */

TArray<APlayerStart*> ATeloGameModeBase::GetPlayerStartsByTag(const FName& InTag) const
{
	TArray<APlayerStart*> Result;

	if (!GetWorld() || InTag.IsNone())
	{
		return Result;
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundActors);

	// Tag가 일치하는 PlayerStart들을 수집
	for (AActor* FoundActor : FoundActors)
	{
		APlayerStart* PlayerStart = Cast<APlayerStart>(FoundActor);
		if (!PlayerStart)
		{
			continue;
		}

		if (PlayerStart->ActorHasTag(InTag))
		{
			Result.Add(PlayerStart);
		}
	}

	return Result;
}

APlayerStart* ATeloGameModeBase::GetRandomPlayerStartByTag(const FName& InTag) const
{
	TArray<APlayerStart*> Candidates = GetPlayerStartsByTag(InTag); // Tag가 일치하는 PlayerStart들을 수집
	if (Candidates.IsEmpty())
	{
		return nullptr;
	}

	// Tag가 일치하는 PlayerStart 중 하나를 랜덤 반환
	const int32 RandomIndex = FMath::RandRange(0, Candidates.Num() - 1);
	return Candidates[RandomIndex];
}
