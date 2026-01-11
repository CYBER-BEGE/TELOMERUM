// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloLockOnComponent.h"
#include "TeloEnemyCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UTeloLockOnComponent::UTeloLockOnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UTeloLockOnComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UTeloLockOnComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ClearLockOn();
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void UTeloLockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 락온 상태가 아니면 종료
	if (!IsLockOn())
		return;

	// 타겟 유효성 체크
	ATeloEnemyCharacter* CurTarget = Target.Get();
	if (!CurTarget)
	{
		ClearLockOn();
		return;
	}

	// 오너 캐릭터 유효성 체크
	const ACharacter* OwnerChar = GetOwnerCharacter();
	if (!OwnerChar)
	{
		ClearLockOn();
		return;
	}

	// 거리 초과 시 해제
	const float Dist = FVector::Dist(OwnerChar->GetActorLocation(), CurTarget->GetActorLocation());
	if (Dist > BreakDistance)
	{
		ClearLockOn();
		return;
	}

	// LOS(LineOfSight) 체크
	if (!HasLineOfSightToTarget(CurTarget)) // 가려져 있다면
	{
		LoseSightAccum += DeltaTime; // 끊긴 시간 누적
		if (LoseSightAccum >= LoseSightGraceTime) // 최대 시간 초과 시 해제
		{
			ClearLockOn();
			return;
		}
	}
	else
	{
		LoseSightAccum = 0.0f;
	}

	// 컨트롤러 회전 업데이트
	UpdateControlRotationToTarget(DeltaTime);
}

// 락온 상태를 켜거나/끄는 함수
void UTeloLockOnComponent::ToggleLockOn()
{
	// 이미 락온 상태면 해제
	if (IsLockOn())
	{
		ClearLockOn();
		return;
	}

	ATeloEnemyCharacter* Best = FindBestTarget(); // 최적 타겟 찾기
	if (!Best) // 없으면 종료
		return;

	Target = Best;
	bIsLockOn = true;
	LoseSightAccum = 0.0f;

	// 락온 시 이동 모드 적용
	ApplyLockOnMovementMode(true);
}

// 락온 강제 해제
void UTeloLockOnComponent::ClearLockOn()
{
	if (!bIsLockOn && !Target.IsValid())
		return;

	Target.Reset();
	bIsLockOn = false;
	LoseSightAccum = 0.0f;

	ApplyLockOnMovementMode(false);
}

// 현재 타겟의 락온 포인트 월드 위치
FVector UTeloLockOnComponent::GetTargetPointWorldLocation() const
{
	ATeloEnemyCharacter* CurTarget = Target.Get();
	return CurTarget ? CurTarget->GetLockOnPointLocation() : FVector::ZeroVector;
}

// 소유자 캐릭터 반환
ACharacter* UTeloLockOnComponent::GetOwnerCharacter() const
{
	return Cast<ACharacter>(GetOwner());
}

// 소유자 플레이어 컨트롤러 반환
APlayerController* UTeloLockOnComponent::GetOwnerPlayerController() const
{
	ACharacter* OwnerChar = GetOwnerCharacter();
	return OwnerChar ? Cast<APlayerController>(OwnerChar->GetController()) : nullptr;
}

// 락온 시 캐릭터 이동 모드 적용/복구
void UTeloLockOnComponent::ApplyLockOnMovementMode(bool bEnable)
{
	ACharacter* OwnerChar = GetOwnerCharacter();
	if (!OwnerChar)
		return;

	UCharacterMovementComponent* MoveComp = OwnerChar->GetCharacterMovement();
	if (!MoveComp)
		return;

	if (bEnable)
	{
		// 복구용 저장
		bPrevUseControllerYaw = OwnerChar->bUseControllerRotationYaw;
		bPrevOrientToMovement = MoveComp->bOrientRotationToMovement;

		// 락온: 스트레이프 모드(컨트롤러 yaw를 캐릭터 yaw로 사용)
		OwnerChar->bUseControllerRotationYaw = true;
		MoveComp->bOrientRotationToMovement = false;
	}
	else
	{
		OwnerChar->bUseControllerRotationYaw = bPrevUseControllerYaw;
		MoveComp->bOrientRotationToMovement = bPrevOrientToMovement;
	}
}

// 타겟에 대한 시야 확보 여부 반환
bool UTeloLockOnComponent::HasLineOfSightToTarget(ATeloEnemyCharacter* InTarget) const
{
	if (!InTarget)
		return false;

	APlayerController* PC = GetOwnerPlayerController();
	ACharacter* OwnerChar = GetOwnerCharacter();
	if (!PC || !OwnerChar)
		return false;

	FVector ViewLoc;	// 카메라 위치
	FRotator ViewRot;	// 카메라 회전
	PC->GetPlayerViewPoint(ViewLoc, ViewRot); // 카메라 위치/회전 얻기

	const FVector TargetLoc = InTarget->GetLockOnPointLocation();

	// 라인 트레이스 제외 설정
	FCollisionQueryParams Params(SCENE_QUERY_STAT(LockOnLOS), false, OwnerChar);
	Params.AddIgnoredActor(InTarget); // 타겟은 무시

	// 라인 트레이스 실행
	FHitResult Hit;
	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		ViewLoc,
		TargetLoc,
		ECC_Visibility,
		Params
	);

	// 안 맞았으면 가려짐
	if (!bHit)
		return true;

	// 맞았으면 타겟인지 확인
	return Hit.GetActor() == InTarget;
}

// 최적의 타겟 찾기
ATeloEnemyCharacter* UTeloLockOnComponent::FindBestTarget() const
{
	APlayerController* PC = GetOwnerPlayerController();
	ACharacter* OwnerChar = GetOwnerCharacter();
	if (!PC || !OwnerChar)
		return nullptr;

	FVector ViewLoc;
	FRotator ViewRot;
	PC->GetPlayerViewPoint(ViewLoc, ViewRot);

	const FVector ViewForward = ViewRot.Vector(); // 카메라 정면 벡터
	const float MaxAngleRad = FMath::DegreesToRadians(MaxLockOnAngleDeg); // 최대 각도 라디안

	// 주변 적 수집 (월드의 ATeloEnemyCharacter 전부 중 거리/각도 필터)
	TArray<AActor*> Found;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATeloEnemyCharacter::StaticClass(), Found); // 월드의 모든 적 수집

	ATeloEnemyCharacter* BestTarget = nullptr;
	float BestScore = TNumericLimits<float>::Max(); // 최저 점수 초기화

	// 수집된 적들 중에서 최적 타겟 탐색
	for (AActor* Actor : Found)
	{
		ATeloEnemyCharacter* Enemy = Cast<ATeloEnemyCharacter>(Actor); // 적 캐릭터로 캐스팅
		if (!Enemy) // 캐스팅 실패 시 해당 회차 무시
			continue;

		// 오너와 적 간 거리
		const float Dist = FVector::Dist(OwnerChar->GetActorLocation(), Enemy->GetActorLocation()); 
		if (Dist > SearchRadius) // 탐색 반경 초과 시 해당 회차 무시
			continue;

		// 카메라 정면 기준 각도 계산
		const FVector LockPoint = Enemy->GetLockOnPointLocation();	// 적의 락온 포인트 위치
		const FVector ToTarget = (LockPoint - ViewLoc);				// 카메라에서 타겟으로 향하는 벡터
		const float ToTargetLen = ToTarget.Size();					// 타겟까지 거리
		if (ToTargetLen < KINDA_SMALL_NUMBER) // 너무 가까우면 해당 회차 무시
			continue;

		// 각도 계산
		const FVector Dir = ToTarget / ToTargetLen;
		const float Angle = FMath::Acos(FMath::Clamp(FVector::DotProduct(ViewForward, Dir), -1.0f, 1.0f));
		if (Angle > MaxAngleRad) // 최대 각도 초과 시 해당 회차 무시
			continue;

		if (!HasLineOfSightToTarget(Enemy)) // 시야 확보 불가 시 해당 회차 무시
			continue;

		// 점수: 각도 우선, 그 다음 거리 (각도 작은 게 최우선)
		const float Score = Angle * 1000.0f + Dist;
		if (Score < BestScore) // 최저 점수 갱신
		{
			BestScore = Score;
			BestTarget = Enemy;
		}
	}

	return BestTarget;
}

// 컨트롤러 회전을 타겟 쪽으로 보정
void UTeloLockOnComponent::UpdateControlRotationToTarget(float DeltaTime)
{
	APlayerController* PC = GetOwnerPlayerController();
	if (!PC)
		return;

	ATeloEnemyCharacter* CurTarget = Target.Get(); // 현재 타겟
	if (!CurTarget)
		return;

	// 카메라 위치/회전 얻기
	FVector ViewLoc;
	FRotator ViewRot;
	PC->GetPlayerViewPoint(ViewLoc, ViewRot);

	// 타겟 위치를 바라보는 회전 계산
	const FVector TargetLoc = CurTarget->GetLockOnPointLocation(); // 타겟 위치
	FRotator Desired = UKismetMathLibrary::FindLookAtRotation(ViewLoc, TargetLoc); // 타겟을 바라보는 회전

	// Pitch 제한(너무 위/아래로 꺾이는 거 방지)
	Desired.Pitch = FMath::ClampAngle(Desired.Pitch, MinPitch, MaxPitch);

	// 현재 회전에서 부드럽게 보간
	const FRotator Current = PC->GetControlRotation(); // 현재 회전
	const FRotator NewRot = FMath::RInterpTo(Current, Desired, DeltaTime, RotationInterpSpeed); // 선형 보간

	// 회전 적용
	PC->SetControlRotation(NewRot);
}
