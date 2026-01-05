// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloDamageable.h"
#include "TeloTrapBase.h"

// Sets default values
ATeloTrapBase::ATeloTrapBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Root 지정
	RootComponent = Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	// Mesh 컴포넌트와 충돌 이벤트 바인딩
	Mesh->OnComponentHit.AddDynamic(this, &ATeloTrapBase::OnHit);
}

// Called when the game starts or when spawned
void ATeloTrapBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATeloTrapBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATeloTrapBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// ITeloDamageable 인터페이스가 구현된 액터 = 데미지 줄 수 있는 액터
	if (ITeloDamageable* Damageable = Cast<ITeloDamageable>(OtherActor))
	{
		Damageable->ApplyDamage(Damage, this, Hit.ImpactPoint, FVector::ZeroVector);
	}
}

