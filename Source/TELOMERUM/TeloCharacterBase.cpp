// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloCharacterBase.h"
#include "Engine/DamageEvents.h"

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
	// 공격에 맞은 대상 저장
	TArray<FHitResult> OutHits;

	// 소켓 시작점에서 정면 Sweep 
	const FVector TraceStart = GetMesh()->GetSocketLocation(DamageSourceBone);
	const FVector TraceEnd = TraceStart + (GetActorForwardVector() * AttackDistance);

	DrawAttackDebug(TraceStart, TraceEnd); // 공격 디버그

	// 공격 가능한 오브젝트 종류
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic); // 적의 오브젝트 파괴 가능여부 논의 필요

	// 공격 범위의 형태(Sphere)
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(AttackRange);

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
}

void ATeloCharacterBase::DrawAttackDebug(FVector TraceStart, FVector TraceEnd)
{
	const FVector CapsuleCenter = (TraceStart + TraceEnd) * 0.5f;
	const float HalfHeight = FVector::Dist(TraceStart, TraceEnd) * 0.5f;
	const FQuat CapsuleRotation = FRotationMatrix::MakeFromZ(TraceEnd - TraceStart).ToQuat();

	// 시작점(파랑)
	DrawDebugSphere(GetWorld(), TraceStart, AttackRange, 16, FColor::Green, false, 5.0f);

	// 끝점(빨강)
	DrawDebugSphere(GetWorld(), TraceEnd, AttackRange, 16, FColor::Red, false, 5.0f);

	// 공격방향(노랑)
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Yellow, false, 5.0f, 0, 2.0f);

	// Sweep 경로(파랑)
	DrawDebugCapsule(GetWorld(), CapsuleCenter, HalfHeight, AttackRange, CapsuleRotation, FColor::Blue, false, 5.0f);
}

void ATeloCharacterBase::DrawHitDebug(const FHitResult& Hit)
{
	// 히트 지점(빨강)
	DrawDebugPoint(GetWorld(), Hit.ImpactPoint, 12.0f, FColor::Red, false, 5.0f);

	// 히트 대상이 보고 있는 방향(하늘)
	DrawDebugLine(GetWorld(), Hit.ImpactPoint, Hit.ImpactPoint + Hit.ImpactNormal * 50.0f, FColor::Cyan, false, 5.0f, 0, 2.0f);
}

void ATeloCharacterBase::HitActor(const FHitResult& HitResult)
{
	// virtual function
}