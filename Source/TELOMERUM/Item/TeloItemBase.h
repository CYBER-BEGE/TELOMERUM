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


	/* =============== 아이템 기존 정보 =============== */

	/* 아이템 ID */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FName ItemID = NAME_None;

	/* 아이템 이름 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText ItemName = FText::GetEmpty();

	/* 아이템 설명 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText ItemDescription = FText::GetEmpty();

	/* 아이템 아이콘 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	class UTexture2D* ItemIcon = nullptr;

	/* 아이템 개수 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item", meta = (ClampMin = "1"))
	int32 ItemCount = 1;


	/* =============== 아이템 사용 관련 =============== */

	/* 아이템이 사용 가능한지 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Use")
	bool bUsable = false;

	/* 아이템 사용 시 소비되는지 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Use")
	bool bConsumeOnUse = true;

	/* 아이템 사용 타입 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Use")
	ETeloItemUseType UseType = ETeloItemUseType::None;

	/* 아이템 사용 시 효과값 (예: HP 회복량) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Use")
	float UseValue = 0.0f;


	/* =============== 상호작용 관련 =============== */

	/* 아이템이 상호작용 가능한지 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Property")
	bool bCanInteract = true;

	/* 아이템이 이미 습득된 상태인지 여부 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Property")
	bool bIsPickedUp = false;

	/* 상호작용 UI에 표시할 문구 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Property")
	FText InteractText = FText::FromString(TEXT("E : 습득"));

public:
	/* 상호작용 가능한 상태인지 */
	virtual bool CanInteract(AActor* Interactor) const override;

	/* 상호작용 실행 */
	virtual void Interact(AActor* Interactor) override;

	/* UI에 표시할 상호작용 문구 */
	virtual FText GetInteractText() const override;

	/* 인벤토리 데이터 기준으로 월드 아이템 정보를 다시 초기화 */
	void InitializeFromInventoryItem(const struct FTeloInventoryItem& InItemData);
};
