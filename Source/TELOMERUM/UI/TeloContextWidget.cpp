// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TeloContextWidget.h"
#include "UI/TeloContextEntryWidget.h"
#include "Components/VerticalBox.h"

void UTeloContextWidget::SetMenuData(const FTeloContextData& InMenuData)
{
	ClearEntries();

	if (!ContextEntryWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UTeloContextWidget] ContextEntryWidgetClass is NULL"));
	}

	if (!ActionListBox)
	{
		return;
	}

	for (const FTeloContextAction& Action : InMenuData.Actions)
	{
		AddEntry(Action);
	}
}

void UTeloContextWidget::ClearEntries()
{
	if (ActionListBox)
	{
		ActionListBox->ClearChildren();
	}
}

void UTeloContextWidget::AddEntry(const FTeloContextAction& InActionData)
{
	if (!ContextEntryWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UTeloContextWidget] ContextEntryWidgetClass is NULL"));
	}

	if (!ActionListBox)
	{
		return;
	}

	UTeloContextEntryWidget* EntryWidget =
		CreateWidget<UTeloContextEntryWidget>(GetOwningPlayer(), ContextEntryWidgetClass);

	if (!EntryWidget)
	{
		return;
	}

	EntryWidget->SetActionData(InActionData); // 메뉴 항목 데이터를 설정
	EntryWidget->OnEntryClicked.AddUObject(this, &UTeloContextWidget::HandleEntryClicked); // 메뉴 항목이 클릭되었을 때의 콜백 등록

	ActionListBox->AddChild(EntryWidget); // VerticalBox에 메뉴 항목 위젯 추가
}

void UTeloContextWidget::HandleEntryClicked(FName ActionID)
{
	OnActionClicked.Broadcast(ActionID);
}
