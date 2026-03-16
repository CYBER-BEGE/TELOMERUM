// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TeloInteractComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/TeloInteractable.h"

// Sets default values for this component's properties
UTeloInteractComponent::UTeloInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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
	InteractSphere->SetupAttachment(OwnerCharacter->GetRootComponent());
	InteractSphere->RegisterComponent();

	InteractSphere->SetSphereRadius(InteractRadius);
	InteractSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractSphere->SetCollisionObjectType(ECC_WorldDynamic);
	InteractSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractSphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
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
		}
	}
}
