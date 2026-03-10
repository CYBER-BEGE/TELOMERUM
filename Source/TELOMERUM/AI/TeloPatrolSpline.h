// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TeloPatrolSpline.generated.h"

UCLASS()
class TELOMERUM_API ATeloPatrolSpline : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeloPatrolSpline();

private:
	/** Variables **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USplineComponent* SplineComponent;

public:
	/** Functions **/
	int32 GetTotalPoints() const;
	int32 GetNextIndexLoop(int32 CurrentIndex) const;
	int32 GetNextIndexPingPong(int32 CurrentIndex, bool& Direction) const; // Direction이 true면 정방향, false면 역방향
	FVector GetPatrolPointLocation(int32 Index) const;

};
