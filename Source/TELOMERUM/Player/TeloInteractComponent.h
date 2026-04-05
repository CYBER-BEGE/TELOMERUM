// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "TeloInteractComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TELOMERUM_API UTeloInteractComponent : public UActorComponent
{
	GENERATED_BODY()


	/* ==================== ActorComponent Lifecycle ==================== */
public:	
	// Sets default values for this component's properties
	UTeloInteractComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	/* ==================== Interact Component Properties ==================== */
private:
	/* 플레이어가 아이템과 접촉하는 상호작용 콜리전 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interact", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* InteractSphere;

	/* 현재 상호작용 중인 액터 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interact", meta = (AllowPrivateAccess = "true"))
	AActor* CurrentInteractActor = nullptr;

	/* 상호작용 반경 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact", meta = (AllowPrivateAccess = "true"))
	float InteractRadius = 150.0f;

	/* 상호작용 UI 위젯 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact|UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UTeloInteractWidget> InteractWidgetClass;


	/* ===================== Interact Component State ==================== */
private:
	/* 현재 범위 안에 들어와 있는 상호작용 후보들 */
	UPROPERTY()
	TArray<AActor*> InteractCandidateList;

	/* 상호작용 UI 위젯 인스턴스 */
	UPROPERTY()
	class UTeloInteractWidget* InteractWidgetInstance = nullptr;


	/* ===================== Overlap Event ==================== */
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


	/* ===================== Internal Functions ==================== */
private:
	/* 현재 컴포넌트가 붙어있는 캐릭터 */
	class ACharacter* GetOwnerCharacter() const;
	/* 현재 컴포넌트가 붙어있는 플레이어 컨트롤러 */
	class APlayerController* GetOwnerPlayerController() const;

	/* 상호작용 UI 위젯 생성 및 초기화 */
	void CreateInteractWidget();
	/* 상호작용 UI 위젯 표시 */
	void ShowInteractWidget(AActor* InteractActor);
	/* 상호작용 UI 위젯 제거 */
	void HideInteractWidget();

	/* 상호작용 가능한 유효한 액터인지 검사 */
	bool IsValidInteractActor(AActor* Actor) const;
	/* 후보 목록을 정리하고 가장 가까운 액터를 현재 대상으로 갱신 */
	void RefreshCurrentInteractActor();


	/* ===================== Interact Component Setup ==================== */
public:
	/* 다른 전체 화면 UI가 열려 있는 동안 상호작용 위젯 표시를 막을지 여부 */
	bool bSuppressInteractWidget = false;

	/* 현재 상호작용 중인 액터 반환 */
	AActor* GetCurrentInteractActor() const { return CurrentInteractActor; }
	/* 상호작용 시도 */
	void TryInteract();
	/* 상호작용 위젯 표시를 일시적으로 막거나 다시 허용 */
	void SetInteractWidgetSuppressed(bool bSuppressed);
};
