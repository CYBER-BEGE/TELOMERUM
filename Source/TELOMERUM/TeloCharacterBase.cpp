// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloCharacterBase.h"
#include "Engine/DamageEvents.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "TeloWeaponBase.h"

// Sets default values
ATeloCharacterBase::ATeloCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATeloCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (AttackSocketName.IsNone())
		UE_LOG(LogTemp, Warning, TEXT("[%s] AttackSocketName is NULL"), *GetActorLabel());

	if (!AttackMontage)
		UE_LOG(LogTemp, Warning, TEXT("[%s] AttackMontage is NULL"), *GetActorLabel());
}

// Called every frame
void ATeloCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATeloCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATeloCharacterBase::ApplyDamage(float Damage, AActor* DamageCauser, const FVector& DamageLocation, const FVector& DamageImpulse)
{
	if (!bIsDamageable) return;
	bIsDamageable = false;

	FDamageEvent DamageEvent;
	const float ActualDamage = TakeDamage(Damage, DamageEvent, nullptr, DamageCauser);

	GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &ATeloCharacterBase::DamageCooldown, 0.2f, false);
}

float ATeloCharacterBase::GetAttackDistance() const
{
	/*
	if (AttackSocketName.IsNone()) return 0.0f;

	const FVector TraceStart = GetMesh()->GetSocketLocation(AttackSocketName); // 공격 시작점 (소켓 위치)
	const FVector TraceEnd = TraceStart + (GetActorForwardVector() * AttackRange); // 정면으로 공격범위 적용

	// Trace 길이 계산 후 반환
	return FVector::Dist(TraceStart, TraceEnd);*/

	FVector TraceStart;
	FVector TraceEnd;
	float TraceRadius = 0.0f;

	if (WeaponInstance && WeaponInstance->GetAttackTraceData(TraceStart, TraceEnd))
	{
		return FVector::Dist(TraceStart, TraceEnd);
	}

	if (AttackSocketName.IsNone()) return 0.0f;

	TraceStart = GetMesh()->GetSocketLocation(AttackSocketName);
	TraceEnd = TraceStart + (GetActorForwardVector() * AttackRange);

	return FVector::Dist(TraceStart, TraceEnd);
}

void ATeloCharacterBase::AttackRequest(AActor* Target)
{
	DoAttack(Target);
}

float ATeloCharacterBase::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (NowHP <= 0.0f) return 0.0f;

	UE_LOG(LogTemp, Display, TEXT("[%s] %s에게 피격당함"), *this->GetActorLabel(), *DamageCauser->GetActorLabel());
	
	NowHP -= Damage;
	// 방어 데미지 감소

	if (NowHP > 0.0f) 
	{
		// HP UI 업데이트
		UE_LOG(LogTemp, Display, TEXT("[%s] HP: %f"), *this->GetActorLabel(), NowHP);
		
		// 피격 약넉백?
	}
	else 
	{
		// 사망
		UE_LOG(LogTemp, Warning, TEXT("죽었습니다"));
	}

	return 0.0f;
}

void ATeloCharacterBase::DamageCooldown()
{
	bIsDamageable = true;
}

void ATeloCharacterBase::TraceAttack(FName DamageSourceBone)
{
	/*
	// 공격에 맞은 대상 저장
	TArray<FHitResult> OutHits;

	// 소켓 시작점에서 정면 Sweep 
	const FVector TraceStart = GetMesh()->GetSocketLocation(DamageSourceBone);
	const FVector TraceEnd = TraceStart + (GetActorForwardVector() * AttackRange);

	DrawAttackDebug(TraceStart, TraceEnd); // 공격 디버그

	// 공격 가능한 오브젝트 종류
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic); // 적의 오브젝트 파괴 가능여부 논의 필요

	// 공격 범위의 형태(Sphere)
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(AttackSize);

	// 공격자 자신의 공격 무시
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	if (GetWorld()->SweepMultiByObjectType(OutHits, TraceStart, TraceEnd, FQuat::Identity, ObjectParams, CollisionShape, QueryParams))
	{
		for (const FHitResult& Hit : OutHits)
		{
			DrawHitDebug(Hit); // 히트 디버그
			HitActor(Hit);
		}
	}
	*/

	UE_LOG(LogTemp, Warning, TEXT("트레이스"));

	TArray<FHitResult> OutHits;

	FVector TraceStart;
	FVector TraceEnd;
	float TraceRadius = AttackSize;

	// 1순위: 무기 기준
	if (WeaponInstance && WeaponInstance->GetAttackTraceData(TraceStart, TraceEnd))
	{
	}
	// 2순위: 기존 캐릭터 기준 fallback
	else
	{
		if (DamageSourceBone.IsNone()) return;

		TraceStart = GetMesh()->GetSocketLocation(DamageSourceBone);
		TraceEnd = TraceStart + (GetActorForwardVector() * AttackRange);
		TraceRadius = AttackSize;
	}

	DrawAttackDebug(TraceStart, TraceEnd);

	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(TraceRadius);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	if (WeaponInstance)
	{
		QueryParams.AddIgnoredActor(WeaponInstance);
	}

	if (GetWorld()->SweepMultiByObjectType(
		OutHits,
		TraceStart,
		TraceEnd,
		FQuat::Identity,
		ObjectParams,
		CollisionShape,
		QueryParams))
	{
		for (const FHitResult& Hit : OutHits)
		{
			DrawHitDebug(Hit);
			HitActor(Hit);
		}
	}
}

void ATeloCharacterBase::BeginAttackTrace()
{
	if (!WeaponInstance) return;
	UE_LOG(LogTemp, Display, TEXT("Trace 시작"));

	FVector TraceStart;
	FVector TraceEnd;

	if (!WeaponInstance->GetAttackTraceData(TraceStart, TraceEnd))
	{
		UE_LOG(LogTemp, Display, TEXT("탈출됨"));
		return;
	}

	bAttackTracing = true;
	PreviousAttackTraceStart = TraceStart;
	PreviousAttackTraceEnd = TraceEnd;
	HitActorsThisSwing.Empty();
}

void ATeloCharacterBase::TickAttackTrace()
{
	if (!bAttackTracing) return;
	if (!WeaponInstance) return;

	FVector CurrentTraceStart;
	FVector CurrentTraceEnd;
	float TraceRadius = 0.0f;

	if (!WeaponInstance->GetAttackTraceData(CurrentTraceStart, CurrentTraceEnd))
	{
		return;
	}

	TArray<FHitResult> OutHits;

	// 디버그: 이전 프레임/현재 프레임의 양 끝점과 연결선
	DrawAttackDebug(PreviousAttackTraceStart, CurrentTraceStart); // 시작점 궤적
	DrawAttackDebug(PreviousAttackTraceEnd, CurrentTraceEnd);     // 끝점 궤적
	DrawAttackDebug(PreviousAttackTraceStart, PreviousAttackTraceEnd); // 이전 프레임 검 선분
	DrawAttackDebug(CurrentTraceStart, CurrentTraceEnd);               // 현재 프레임 검 선분

	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(AttackSize);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(WeaponInstance);

	// 1. 시작 소켓: 이전 -> 현재
	GetWorld()->SweepMultiByObjectType(
		OutHits,
		PreviousAttackTraceStart,
		CurrentTraceStart,
		FQuat::Identity,
		ObjectParams,
		CollisionShape,
		QueryParams
	);

	// 2. 끝 소켓: 이전 -> 현재
	GetWorld()->SweepMultiByObjectType(
		OutHits,
		PreviousAttackTraceEnd,
		CurrentTraceEnd,
		FQuat::Identity,
		ObjectParams,
		CollisionShape,
		QueryParams
	);

	// 3. 이전 프레임의 검 선분
	GetWorld()->SweepMultiByObjectType(
		OutHits,
		PreviousAttackTraceStart,
		PreviousAttackTraceEnd,
		FQuat::Identity,
		ObjectParams,
		CollisionShape,
		QueryParams
	);

	// 4. 현재 프레임의 검 선분
	GetWorld()->SweepMultiByObjectType(
		OutHits,
		CurrentTraceStart,
		CurrentTraceEnd,
		FQuat::Identity,
		ObjectParams,
		CollisionShape,
		QueryParams
	);

	for (const FHitResult& Hit : OutHits)
	{
		AActor* HitActorPtr = Hit.GetActor();
		if (!HitActorPtr) continue;

		if (HitActorsThisSwing.Contains(HitActorPtr))
		{
			continue;
		}

		HitActorsThisSwing.Add(HitActorPtr);

		DrawHitDebug(Hit);
		HitActor(Hit);
	}

	PreviousAttackTraceStart = CurrentTraceStart;
	PreviousAttackTraceEnd = CurrentTraceEnd;
}

void ATeloCharacterBase::EndAttackTrace()
{
	if (!bAttackTracing) return;

	bAttackTracing = false;
	HitActorsThisSwing.Empty();
}
void ATeloCharacterBase::DrawAttackDebug(FVector TraceStart, FVector TraceEnd)
{
	/*
	const FVector CapsuleCenter = (TraceStart + TraceEnd) * 0.5f;
	const float HalfHeight = FVector::Dist(TraceStart, TraceEnd) * 0.5f;
	const FQuat CapsuleRotation = FRotationMatrix::MakeFromZ(TraceEnd - TraceStart).ToQuat();

	// 시작점(파랑)
	DrawDebugSphere(GetWorld(), TraceStart, AttackSize, 16, FColor::Green, false, 5.0f);

	// 끝점(빨강)
	DrawDebugSphere(GetWorld(), TraceEnd, AttackSize, 16, FColor::Red, false, 5.0f);

	// 공격방향(노랑)
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Yellow, false, 5.0f, 0, 2.0f);

	// Sweep 경로(파랑)
	DrawDebugCapsule(GetWorld(), CapsuleCenter, HalfHeight, AttackSize, CapsuleRotation, FColor::Blue, false, 5.0f);
	*/

	const FVector CapsuleCenter = (TraceStart + TraceEnd) * 0.5f;
	const float HalfHeight = FVector::Dist(TraceStart, TraceEnd) * 0.5f;
	const FQuat CapsuleRotation = FRotationMatrix::MakeFromZ(TraceEnd - TraceStart).ToQuat();

	DrawDebugPoint(GetWorld(), TraceStart, 12.0f, FColor::Green, false, 5.0f);
	DrawDebugPoint(GetWorld(), TraceEnd, 12.0f, FColor::Red, false, 5.0f);
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Yellow, false, 5.0f, 0, 2.0f);
}

void ATeloCharacterBase::DrawHitDebug(const FHitResult& Hit)
{
	// 히트 지점(빨강)
	DrawDebugPoint(GetWorld(), Hit.ImpactPoint, 12.0f, FColor::Red, false, 5.0f);

	// 히트 대상이 보고 있는 방향(하늘)
	DrawDebugLine(GetWorld(), Hit.ImpactPoint, Hit.ImpactPoint + Hit.ImpactNormal * 50.0f, FColor::Cyan, false, 5.0f, 0, 2.0f);
}

void ATeloCharacterBase::DoAttack(AActor* Target)
{
	if (!bCanAttack || bIsAttacking) return;
	if (AttackSocketName.IsNone() || !AttackMontage || !GetMesh()) return;
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance) 
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] NO AnimInstance"), *GetActorLabel())
	}

	UE_LOG(LogTemp, Warning, TEXT("[%s] Attack Start"), *GetActorLabel());

	bCanAttack = false;
	bIsAttacking = true;

	RotateToTarget(Target);
	//TraceAttack(AttackSocketName);

	if(AttackMontage && AnimInstance)
	{
		AnimInstance->Montage_Play(AttackMontage);
		UE_LOG(LogTemp, Warning, TEXT("AttackMontage Asset: %s"), *GetNameSafe(AttackMontage));
	}

	//GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ATeloCharacterBase::DoAttackEnd, AttackSpeed, false);
}

void ATeloCharacterBase::DoAttackEnd()
{
	if (!bIsAttacking) return; // 공격 중이 아닐 시 종료

	bIsAttacking = false;
	bCanAttack = true;

	if (GetWorld())
	{
		GetWorldTimerManager().ClearTimer(AttackTimerHandle);
	}

	OnAttackEnd.Broadcast();
}

void ATeloCharacterBase::HitActor(const FHitResult& HitResult)
{
	ITeloDamageable* Damageable = Cast<ITeloDamageable>(HitResult.GetActor());

	if (Damageable)
	{
		const FVector Impulse = (HitResult.ImpactNormal * -KnockbackImpulse) + (FVector::UpVector * KnockupImpulse);
		Damageable->ApplyDamage(AttackDamage, this, HitResult.ImpactPoint, Impulse);
	}
}

void ATeloCharacterBase::RotateToTarget(const AActor* Target)
{
	if (!Target) return;

	FVector ToTarget = Target->GetActorLocation() - GetActorLocation();
	ToTarget.Z = 0.0f;

	if (!ToTarget.IsNearlyZero())
	{
		const FRotator TargetYaw = ToTarget.Rotation();
		SetActorRotation(FRotator(0.f, TargetYaw.Yaw, 0.f));
	}
}
