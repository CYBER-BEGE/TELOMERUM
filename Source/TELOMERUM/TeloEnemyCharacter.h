// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TeloCharacterBase.h"
#include "TeloEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TELOMERUM_API ATeloEnemyCharacter : public ATeloCharacterBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	ATeloEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	/* 플레이어가 카메라를 락온하는 지점 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* LockOnPoint;

};
