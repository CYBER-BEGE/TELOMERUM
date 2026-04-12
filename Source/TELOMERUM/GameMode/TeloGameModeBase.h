// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "TeloGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TELOMERUM_API ATeloGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	/* ============================== Random PlayerStart ============================== */
protected:
	/* Tag가 일치하는 PlayerStart들을 수집 */
	TArray<class APlayerStart*> GetPlayerStartsByTag(const FName& InTag) const;

	/* Tag가 일치하는 PlayerStart 중 하나를 랜덤 반환 */
	class APlayerStart* GetRandomPlayerStartByTag(const FName& InTag) const;
};
