// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/TeloInteractable.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	bool bCanInteract = true;

public:
	/* 상호작용 가능한 상태인지 */
	virtual bool CanInteract(AActor* Interactor) const override;

	/* 상호작용 실행 */
	virtual void Interact(AActor* Interactor) override;

};
