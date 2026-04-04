// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloPatrolSpline.h"

#include "Components/SplineComponent.h"

// Sets default values
ATeloPatrolSpline::ATeloPatrolSpline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SetRootComponent(SplineComponent);
}

int32 ATeloPatrolSpline::GetTotalNodes() const
{
	if (!SplineComponent) return 0;
	return SplineComponent->GetNumberOfSplinePoints();
}

int32 ATeloPatrolSpline::GetNextIndexLoop(int32 CurrentIndex) const
{
	const int32 Total = GetTotalNodes();
	if (Total <= 0) return INDEX_NONE;
	if (Total == 1) return 0;

	const int32 SafeCurrentIndex = FMath::Clamp(CurrentIndex, 0, Total - 1); // 유효 범위로 인덱스 제한
	return (SafeCurrentIndex + 1) % Total; // 다음 인덱스로 계속 +1 (마지막 노드 다음은 첫 번째 노드로 돌아감)
}

int32 ATeloPatrolSpline::GetNextIndexPingPong(int32 CurrentIndex, bool& Direction) const
{
	const int32 Total = GetTotalNodes();
	if (Total <= 0)
	{
		Direction = false;
		return INDEX_NONE;
	}
	if (Total == 1) 
	{
		Direction = true;
		return 0;
	}

	const int32 SafeCurrentIndex = FMath::Clamp(CurrentIndex, 0, Total - 1); // 유효 범위로 인덱스 제한
	
	int32 MoveValue = Direction ? 1 : -1; // Direction이 true면 정방향(1), false면 역방향(-1)
	int32 NextIndex = SafeCurrentIndex + MoveValue;

	if (NextIndex >= Total)
	{
		Direction = false;
		NextIndex = Total - 2;
	}
	else if (NextIndex < 0)
	{
		Direction = true;
		NextIndex = 1;
	}

	return NextIndex;
}

FVector ATeloPatrolSpline::GetPatrolNodeLocation(int32 Index) const
{
	if (!SplineComponent) return GetActorLocation();

	const int32 Total = SplineComponent->GetNumberOfSplinePoints();
	if (Total <= 0) return GetActorLocation();

	const int32 SafeIndex = FMath::Clamp(Index, 0, Total - 1);
	return SplineComponent->GetLocationAtSplinePoint(SafeIndex, ESplineCoordinateSpace::World);
}

