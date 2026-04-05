// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TeloInteractComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

#include "Components/SphereComponent.h"

#include "Interfaces/TeloInteractable.h"
#include "UI/TeloInteractWidget.h"


/* ==================== ActorComponent Lifecycle ==================== */

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
	InteractSphere->OnComponentBeginOverlap.AddDynamic(this, &UTeloInteractComponent::OnInteractBeginOverlap);
	InteractSphere->OnComponentEndOverlap.AddDynamic(this, &UTeloInteractComponent::OnInteractEndOverlap);

	CreateInteractWidget();
	HideInteractWidget();
}

// Called every frame
void UTeloInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


/* ===================== Overlap Event ==================== */

void UTeloInteractComponent::OnInteractBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	// 유효한 상호작용 대상이 범위에 들어왔는지 검사
	if (!IsValidInteractActor(OtherActor))
		return;

	InteractCandidateList.AddUnique(OtherActor); // 후보 목록에 추가 (중복 방지)
	RefreshCurrentInteractActor();
}

void UTeloInteractComponent::OnInteractEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
)
{
	if (OtherActor == nullptr)
		return;

	InteractCandidateList.Remove(OtherActor);
	RefreshCurrentInteractActor();
}


/* ===================== Internal Functions ==================== */

ACharacter* UTeloInteractComponent::GetOwnerCharacter() const
{
	return Cast<ACharacter>(GetOwner());
}

APlayerController* UTeloInteractComponent::GetOwnerPlayerController() const
{
	ACharacter* OwnerCharacter = GetOwnerCharacter();
	return OwnerCharacter ? Cast<APlayerController>(OwnerCharacter->GetController()) : nullptr;
}

void UTeloInteractComponent::CreateInteractWidget()
{
	if (!InteractWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UTeloInteractComponent] InteractWidgetClass이 설정되지 않았습니다."));
		return;
	}

	if (InteractWidgetInstance)
		return;

	APlayerController* OwnerPC = GetOwnerPlayerController();
	if (!OwnerPC)
		return;

	InteractWidgetInstance = CreateWidget<UTeloInteractWidget>(OwnerPC, InteractWidgetClass);
	if (InteractWidgetInstance)
	{
		InteractWidgetInstance->AddToViewport();
	}
}

void UTeloInteractComponent::ShowInteractWidget(AActor* InteractActor)
{
	// 상호작용 UI를 표시할 액터가 유효한지 검사
	if (!InteractWidgetInstance || !InteractActor)
	{
		return;
	}

	// 다른 전체 화면 UI가 열려 있는 동안엔 상호작용 UI 표시 금지
	if (bSuppressInteractWidget)
	{
		HideInteractWidget();
		return;
	}

	// 상호작용 대상이 상호작용 인터페이스를 구현하고 있다면 UI에 텍스트 설정
	if (ITeloInteractable* Interactable = Cast<ITeloInteractable>(InteractActor))
	{
		InteractWidgetInstance->SetInteractText(Interactable->GetInteractText());

		// 입력을 절대 먹지 않도록 표시
		InteractWidgetInstance->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UTeloInteractComponent::HideInteractWidget()
{
	if (InteractWidgetInstance)
	{
		InteractWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
	}
}

bool UTeloInteractComponent::IsValidInteractActor(AActor* Actor) const
{
	ACharacter* OwnerCharacter = GetOwnerCharacter();
	if (!OwnerCharacter) // 소유 캐릭터가 유효하지 않으면
		return false;

	if (!IsValid(Actor)) // 액터가 유효하지 않거나 이미 파괴된 경우
		return false;

	if (Actor == OwnerCharacter) // 상호작용 대상이 자기 자신이라면
		return false;

	if (ITeloInteractable* Interactable = Cast<ITeloInteractable>(Actor)) // 액터가 상호작용 인터페이스를 구현하고 있다면
	{
		return Interactable->CanInteract(OwnerCharacter); // 상호작용이 가능한 경우에만 유효한 상호작용 대상
	}

	return false;
}

void UTeloInteractComponent::RefreshCurrentInteractActor()
{
	ACharacter* OwnerCharacter = GetOwnerCharacter();
	if (!OwnerCharacter)
		return;

	// 죽었거나, 파괴됐거나, 상호작용 불가능해진 후보 제거
	InteractCandidateList.RemoveAll([this](AActor* Actor)
		{
			return !IsValidInteractActor(Actor);
		});

	AActor* NewCurrentActor = nullptr;
	float BestDistSq = TNumericLimits<float>::Max();

	for (AActor* Candidate : InteractCandidateList)
	{
		if (!IsValidInteractActor(Candidate))
			continue;

		const float DistSq = FVector::DistSquared(
			OwnerCharacter->GetActorLocation(),
			Candidate->GetActorLocation()
		);

		if (DistSq < BestDistSq)
		{
			BestDistSq = DistSq;
			NewCurrentActor = Candidate;
		}
	}

	CurrentInteractActor = NewCurrentActor;

	if (CurrentInteractActor)
	{
		ShowInteractWidget(CurrentInteractActor);
	}
	else
	{
		HideInteractWidget();
	}
}


/* ===================== Interact Component Setup ==================== */

void UTeloInteractComponent::TryInteract()
{
	ACharacter* OwnerCharacter = GetOwnerCharacter();
	if (!OwnerCharacter)
		return;

	RefreshCurrentInteractActor();

	if (CurrentInteractActor == nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("[UTeloInteractComponent] 현재 상호작용 가능한 액터가 없습니다."));
		return;
	}

	if (ITeloInteractable* Interactable = Cast<ITeloInteractable>(CurrentInteractActor))
	{
		if (Interactable->CanInteract(OwnerCharacter))
		{
			Interactable->Interact(OwnerCharacter);

			// 상호작용 후 파괴되거나 상태가 바뀔 수 있으니 다시 계산
			RefreshCurrentInteractActor();
		}
	}
}

void UTeloInteractComponent::SetInteractWidgetSuppressed(bool bSuppressed)
{
	bSuppressInteractWidget = bSuppressed;

	if (bSuppressInteractWidget)
	{
		HideInteractWidget();
		return;
	}

	// 다시 허용되면 현재 주변 상호작용 후보를 기준으로 UI 재계산
	RefreshCurrentInteractActor();
}
