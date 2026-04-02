// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/TeloInteractable.h"
#include "Item/TeloItemTypes.h"
#include "TeloItemBase.generated.h"

UCLASS()
class TELOMERUM_API ATeloItemBase : public AActor, public ITeloInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeloItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;


	/* 아이템 기존 정보 */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FName ItemID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText ItemName = FText::GetEmpty();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText ItemDescription = FText::GetEmpty();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	class UTexture2D* ItemIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item", meta = (ClampMin = "1"))
	int32 ItemCount = 1;


	/* 아이템 사용 관련 */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Use")
	bool bUsable = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Use")
	bool bConsumeOnUse = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Use")
	ETeloItemUseType UseType = ETeloItemUseType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Use")
	float UseValue = 0.0f;


	/* 상호작용 관련 */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Property")
	bool bCanInteract = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Property")
	bool bIsPickedUp = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Property")
	FText InteractText = FText::FromString(TEXT("E : 습득"));

public:
	/* 상호작용 가능한 상태인지 */
	virtual bool CanInteract(AActor* Interactor) const override;

	/* 상호작용 실행 */
	virtual void Interact(AActor* Interactor) override;

	/* UI에 표시할 상호작용 문구 */
	virtual FText GetInteractText() const override;
};
