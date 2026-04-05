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
	/* 순찰 경로 SplineComponent */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USplineComponent* SplineComponent;

public:
	/* 총 노드 개수를 구하는 함수 */
	int32 GetTotalNodes() const;

	/* 순환 순찰 */
	int32 GetNextIndexLoop(int32 CurrentIndex) const;

	/* 왕복 순찰 - 끝 노드에 도달하면 방향 전환 (Direction이 true면 정방향, false면 역방향) */
	int32 GetNextIndexPingPong(int32 CurrentIndex, bool& Direction) const;
	
	/* 인덱스에 해당하는 노드 위치 반환 */
	FVector GetPatrolNodeLocation(int32 Index) const;

};
