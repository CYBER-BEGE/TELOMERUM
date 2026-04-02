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
	
	// Weapon Attach 컴포넌트 생성
	WeaponAttachComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponAttachComponent"));
	WeaponAttachComponent->SetupAttachment(GetMesh());
}

// Called when the game starts or when spawned
void ATeloCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	// Weapon Attach Component를 Weapon Socket에 부착
	if (!IsWeaponComponentAttached()) return;
	
	// Weapon 생성 및 장착
	ATeloWeaponBase* Weapon = SpawnWeapon(WeaponClass);
	if (Weapon)
	{
		EquipWeapon(Weapon);
	}

	//if (!AttackMontage) UE_LOG(LogTemp, Warning, TEXT("[%s] AttackMontage is NULL"), *GetActorLabel());
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

	if (WeaponSocketName.IsNone()) return 0.0f;

	TraceStart = GetMesh()->GetSocketLocation(WeaponSocketName);
	TraceEnd = TraceStart + (GetActorForwardVector() * AttackRange);

	return FVector::Dist(TraceStart, TraceEnd);
}

void ATeloCharacterBase::AttackRequest(AActor* Target)
{
	StartAttack(Target);
}

bool ATeloCharacterBase::IsWeaponComponentAttached()
{
	if (!WeaponAttachComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] WeaponAttachComponent 없음"), *GetActorLabel());
		return false;
	}

	if (!GetMesh())
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] Mesh 없음"), *GetActorLabel());
		return false;
	}

	if (WeaponSocketName.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] WeaponSocketName 없음"), *GetActorLabel());
		return false;
	}

	WeaponAttachComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);
	return true;
}

ATeloWeaponBase* ATeloCharacterBase::SpawnWeapon(TSubclassOf<class ATeloWeaponBase> NewWeaponClass)
{
	if (!NewWeaponClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] 스폰하려는 WeaponClass 없음"), *GetActorLabel());
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	return GetWorld()->SpawnActor<ATeloWeaponBase>(NewWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
}

bool ATeloCharacterBase::EquipWeapon(ATeloWeaponBase* NewWeapon)
{
	if (!NewWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] 생성된 Weapon 객체 없음"), *GetActorLabel());
		return false;
	}

	if (!WeaponAttachComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] WeaponAttachComponent 없음"), *GetActorLabel());
		return false;
	}

	NewWeapon->AttachToComponent(WeaponAttachComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	WeaponInstance = NewWeapon;

	return true;
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

void ATeloCharacterBase::StartAttackTrace()
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
	AlreadyHitActors.Empty();
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

		if (AlreadyHitActors.Contains(HitActorPtr))
		{
			continue;
		}

		AlreadyHitActors.Add(HitActorPtr);

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
	AlreadyHitActors.Empty();
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

/*
void ATeloCharacterBase::DoAttack(AActor* Target)
{
	if (!bCanAttack || bIsAttacking) return;
	if (WeaponSocketName.IsNone() || !WeaponAnim || !GetMesh()) return;
	
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

	if(/*AttackMontage && AnimInstance)
	{
		AnimInstance->Montage_Play(WeaponInstance->AttackMontage);
		UE_LOG(LogTemp, Warning, TEXT("AttackMontage Asset: %s"), *GetNameSafe(AttackMontage));
	}

	//GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ATeloCharacterBase::DoAttackEnd, AttackSpeed, false);
}*/

void ATeloCharacterBase::StartAttack(AActor* Target)
{
	if (!bCanAttack || bIsAttacking) return;
	if (!WeaponInstance) return;
	if (!GetMesh()) return;

	UE_LOG(LogTemp, Warning, TEXT("[%s] Start Attack"), *GetActorLabel());

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] No AnimInstance"), *GetActorLabel());
		return;
	}

	if (!WeaponInstance->AttackMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] No AttackMontage in WeaponInstance"), *GetActorLabel());
		return;
	}

	bCanAttack = false;
	bIsAttacking = true;

	RotateToTarget(Target);

	AnimInstance->Montage_Play(WeaponInstance->AttackMontage);
}

void ATeloCharacterBase::EndAttack()
{
	if (!bIsAttacking) return; // 공격 중이 아닐 시 종료

	bIsAttacking = false;
	bCanAttack = true;

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
