// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TeloInteractComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TELOMERUM_API UTeloInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTeloInteractComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	/* 플레이어가 아이템과 접촉하는 상호작용 콜리전 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interact", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* InteractSphere;

	/* 상호작용 반경 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact", meta = (AllowPrivateAccess = "true"))
	float InteractRadius = 150.0f;

	/* 현재 상호작용 중인 액터 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interact", meta = (AllowPrivateAccess = "true"))
	AActor* CurrentInteractActor = nullptr;

	/* 현재 범위 안에 들어와 있는 상호작용 후보들 */
	UPROPERTY()
	TArray<AActor*> InteractCandidateList;

	/* 상호작용 UI 위젯 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact|UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UTeloInteractWidget> InteractWidgetClass;

	/* 상호작용 UI 위젯 인스턴스 */
	UPROPERTY()
	class UTeloInteractWidget* InteractWidgetInstance = nullptr;

private:
	/* 상호작용 콜리전이 액터와 겹치기 시작할 때 호출 */
	UFUNCTION()
	void OnInteractBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	/* 상호작용 콜리전이 액터와 겹치기 끝날 때 호출 */
	UFUNCTION()
	void OnInteractEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	/* 현재 컴포넌트가 붙어있는 캐릭터 */
	class ACharacter* GetOwnerCharacter() const;
	class APlayerController* GetOwnerPlayerController() const;

	void CreateInteractWidget();
	void ShowInteractWidget(AActor* InteractActor);
	void HideInteractWidget();

	/* 상호작용 가능한 유효한 액터인지 검사 */
	bool IsValidInteractActor(AActor* Actor) const;

	/* 후보 목록을 정리하고 가장 가까운 액터를 현재 대상으로 갱신 */
	void RefreshCurrentInteractActor();

public:
	/* 현재 상호작용 중인 액터 반환 */
	AActor* GetCurrentInteractActor() const { return CurrentInteractActor; }

	/* 상호작용 시도 */
	void TryInteract();
};
