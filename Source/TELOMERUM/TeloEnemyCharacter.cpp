// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloEnemyCharacter.h"
#include "Components/WidgetComponent.h"

ATeloEnemyCharacter::ATeloEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// LockOnPoint 컴포넌트 생성 및 루트에 부착
	LockOnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("LockOnPoint"));
	LockOnPoint->SetupAttachment(RootComponent);

	// LockOnMarkerWidget 컴포넌트 생성 및 LockOnPoint에 부착
	LockOnMarkerWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockOnMarkerWidget"));
	LockOnMarkerWidget->SetupAttachment(LockOnPoint);
	LockOnMarkerWidget->SetWidgetSpace(EWidgetSpace::Screen);	// 스크린 공간에 렌더링
	LockOnMarkerWidget->SetDrawAtDesiredSize(true);				// 위젯의 원하는 크기로 그리기
	LockOnMarkerWidget->SetPivot(FVector2D(0.5f, 0.5f));		// 피벗을 중앙으로 설정
	LockOnMarkerWidget->SetTwoSided(true);						// 양면 렌더링
	LockOnMarkerWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 충돌 비활성화
	LockOnMarkerWidget->SetGenerateOverlapEvents(false);		// 오버랩 이벤트 비활성화

	// 초기 비활성화 설정
	LockOnMarkerWidget->SetVisibility(false, true);				// 기본적으로 비활성화
	LockOnMarkerWidget->SetHiddenInGame(true);					// 게임 중 숨기기

	// 초기 상태 설정
	MaxHP = 100.0f;
}

// Called when the game starts or when spawned
void ATeloEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATeloEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector ATeloEnemyCharacter::GetLockOnPointLocation() const
{
	// LockOnPoint가 유효하면 그 위치 반환, 아니면 액터 위치 반환
	return LockOnPoint ? LockOnPoint->GetComponentLocation() : GetActorLocation();
}

void ATeloEnemyCharacter::SetLockOnMarkerVisible(bool bVisible)
{
	if (!LockOnMarkerWidget)
		return;

	LockOnMarkerWidget->SetHiddenInGame(!bVisible);
	LockOnMarkerWidget->SetVisibility(bVisible, true);
}