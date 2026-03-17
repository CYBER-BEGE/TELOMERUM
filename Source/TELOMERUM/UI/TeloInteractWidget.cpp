// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TeloInteractWidget.h"
#include "Components/TextBlock.h"

void UTeloInteractWidget::SetInteractText(const FText& NewText)
{
	if (InteractTextBlock)
	{
		InteractTextBlock->SetText(NewText);
	}
}
