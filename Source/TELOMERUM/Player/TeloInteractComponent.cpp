// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TeloInteractComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/TeloInteractable.h"

// Sets default values for this component's properties
UTeloInteractComponent::UTeloInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UTeloInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* OwnerCharacter = GetOwnerCharacter();
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UTeloInteractComponent] Owner is not Character"));
		return;
	}

	InteractSphere = NewObject<USphereComponent>(OwnerCharacter, TEXT("InteractSphere"));
	if (!InteractSphere)
		return;

	InteractSphere->SetupAttachment(OwnerCharacter->GetRootComponent());			// 캐릭터의 루트 컴포넌트에 상호작용 콜리전 부착
	InteractSphere->RegisterComponent();											// 컴포넌트 등록

	InteractSphere->SetSphereRadius(InteractRadius);								// 상호작용 반경 설정
	InteractSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);				// 충돌은 감지하지만 물리적 반응은 없음
	InteractSphere->SetCollisionObjectType(ECC_WorldDynamic);						// 월드 다이나믹으로 설정하여 다른 액터와 충돌 감지
	InteractSphere->SetCollisionResponseToAllChannels(ECR_Ignore);					// 모든 채널에 대해 충돌 무시
	InteractSphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);	// 월드 다이나믹 채널과 겹칠 때 오버랩 이벤트 발생
	InteractSphere->SetGenerateOverlapEvents(true);

	// Overlap 이벤트 바인딩
	InteractSphere->OnComponentBeginOverlap.AddDynamic(this, &UTeloInteractComponent::OnInteractSphereBeginOverlap);
	InteractSphere->OnComponentEndOverlap.AddDynamic(this, &UTeloInteractComponent::OnInteractSphereEndOverlap);
}

// Called every frame
void UTeloInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

ACharacter* UTeloInteractComponent::GetOwnerCharacter() const
{
	return Cast<ACharacter>(GetOwner());
}

void UTeloInteractComponent::OnInteractSphereBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	ACharacter* OwnerCharacter = GetOwnerCharacter();
	if (!OwnerCharacter)
		return;

	if (OtherActor == nullptr || OtherActor == OwnerCharacter)
		return;

	if (ITeloInteractable* Interactable = Cast<ITeloInteractable>(OtherActor))
	{
		if (Interactable->CanInteract(OwnerCharacter))
		{
			CurrentInteractActor = OtherActor;

			UE_LOG(LogTemp, Warning, TEXT("[%s] 상호작용 가능한 아이템이 감지되었습니다."), *OtherActor->GetActorLabel());
		}
	}
}

void UTeloInteractComponent::OnInteractSphereEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
)
{
	ACharacter* OwnerCharacter = GetOwnerCharacter();
	if (!OwnerCharacter)
		return;

	if (OtherActor == nullptr || OtherActor == OwnerCharacter)
		return;

	if (ITeloInteractable* Interactable = Cast<ITeloInteractable>(OtherActor))
	{
		if (Interactable->CanInteract(OwnerCharacter))
		{
			UE_LOG(LogTemp, Warning, TEXT("[%s] 상호작용 가능한 아이템 범위에서 벗어났습니다."), *OtherActor->GetActorLabel());

			CurrentInteractActor = nullptr;
		}
	}
}

void UTeloInteractComponent::TryInteract()
{
	ACharacter* OwnerCharacter = GetOwnerCharacter();
	if (!OwnerCharacter)
		return;

	if (CurrentInteractActor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UTeloInteractComponent] 현재 상호작용 가능한 액터가 없습니다."));
		return;
	}

	if (ITeloInteractable* Interactable = Cast<ITeloInteractable>(CurrentInteractActor))
	{
		if (Interactable->CanInteract(OwnerCharacter))
		{
			Interactable->Interact(OwnerCharacter);
		}
	}
}
