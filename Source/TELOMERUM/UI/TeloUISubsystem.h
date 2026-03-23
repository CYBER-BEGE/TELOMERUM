// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "TeloUISubsystem.generated.h"

/**
 * 
 */
UCLASS()
class TELOMERUM_API UTeloUISubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UTeloInventoryWidget> InventoryWidgetClass;

	UPROPERTY()
	class UTeloInventoryWidget* InventoryWidgetInstance = nullptr;

public:
	/* 인벤토리 위젯 클래스 설정 */
	void SetInventoryWidgetClass(TSubclassOf<class UTeloInventoryWidget> InWidgetClass);

	/* 인벤토리 위젯 표시 */
	void OpenInventory();
	/* 인벤토리 위젯 숨김 */
	void CloseInventory();
	/* 인벤토리 위젯 토글 */
	void ToggleInventory();
	/* 인벤토리 위젯이 열려있는지 여부 반환 */
	bool IsInventoryOpen() const;

private:
	/* 인벤토리 위젯 생성 */
	void CreateInventoryWidget();

	/* UI 입력 모드 적용 */
	void ApplyUIInputMode();
	/* 게임 입력 모드 적용 */
	void ApplyGameInputMode();
};
