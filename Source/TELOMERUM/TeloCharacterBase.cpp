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
	FDamageEvent DamageEvent;
	const float ActualDamage = TakeDamage(Damage, DamageEvent, nullptr, DamageCauser);
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