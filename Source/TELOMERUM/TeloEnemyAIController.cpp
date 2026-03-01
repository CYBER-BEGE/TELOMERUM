// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloEnemyAIController.h"

ATeloEnemyAIController::ATeloEnemyAIController()
{

}

void ATeloEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}
}
