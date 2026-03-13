// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloBTT_AIAttack.h"
#include "AIController.h"
#include "GameFramework/Actor.h"
#include "TeloCharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"

UTeloBTT_AIAttack::UTeloBTT_AIAttack()
{
	NodeName = TEXT("AI Attack");
}

EBTNodeResult::Type UTeloBTT_AIAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComponent.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	APawn* Owner = AIController->GetPawn();
	if (!Owner) return EBTNodeResult::Failed;

	UBlackboardComponent* BlackBoard = OwnerComponent.GetBlackboardComponent();
	if (!BlackBoard) return EBTNodeResult::Failed;

	AActor* TargetActor = Cast<AActor>(BlackBoard->GetValueAsObject(TEXT("TargetActor")));
	if (!TargetActor) return EBTNodeResult::Failed;

	ATeloCharacterBase* OwnerCharacter = Cast<ATeloCharacterBase>(Owner);
	OwnerCharacter->AttackRequest(TargetActor); // 공격 요청

	UE_LOG(LogTemp, Warning, TEXT("[%s] AIAttack: 컨트롤러에서 공격 요청"), *Owner->GetActorLabel());

	return EBTNodeResult::Succeeded;
}
