// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TeloPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TELOMERUM_API ATeloPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ATeloPlayerController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** 입력 매핑 컨텍스트 */
	UPROPERTY(EditAnywhere, Category = "Input Mappings")
	TArray<class UInputMappingContext*> DefaultMappingContexts;

	/** 입력 매핑 컨텍스트 설정 */
	virtual void SetupInputComponent() override;

//private:
//	UPROPERTY(EditAnywhere, Category = "UI")
//	TSubclassOf<class UUserWidget> HUDClass;
//
//	UPROPERTY()
//	UUserWidget* HUD;
};
