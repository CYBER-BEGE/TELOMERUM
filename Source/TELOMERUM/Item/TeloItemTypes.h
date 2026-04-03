// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TeloItemTypes.generated.h"

UENUM(BlueprintType)
enum class ETeloItemUseType : uint8
{
	None			UMETA(DisplayName = "None"),
	HealHP			UMETA(DisplayName = "Heal HP"),
	HealStamina		UMETA(DisplayName = "Heal Stamina")
};

class TELOMERUM_API TeloItemTypes
{
};
