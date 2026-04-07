// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloBTT_AIAttack.h"

#include "AIController.h"
#include "GameFramework/Actor.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "TeloCharacterBase.h"

UTeloBTT_AIAttack::UTeloBTT_AIAttack()
{
	NodeName = TEXT("AI Attack");

	bCreateNodeInstance = true; // 인스턴스별 상태 저장 = true
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
	if (!OwnerCharacter) return EBTNodeResult::Failed;

	CachedOwnerComponent = &OwnerComponent;
	CachedOwnerCharacter = OwnerCharacter;

	ClearDelegate();
	OwnerCharacter->OnAttackEnd.AddUObject(this, &UTeloBTT_AIAttack::HandleAttackEnd);

	AIController->StopMovement();
	OwnerCharacter->AttackRequest(TargetActor); // 공격 요청

	UE_LOG(LogTemp, Warning, TEXT("[%s] AIAttack: 컨트롤러에서 공격 요청"), *Owner->GetActorLabel());

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UTeloBTT_AIAttack::AbortTask(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory)
{
	ClearDelegate();
	CachedOwnerComponent = nullptr;
	CachedOwnerCharacter = nullptr;

	return EBTNodeResult::Aborted;
}

void UTeloBTT_AIAttack::ClearDelegate() const
{
	if (CachedOwnerCharacter)
	{
		CachedOwnerCharacter->OnAttackEnd.RemoveAll(this);
	}
}

void UTeloBTT_AIAttack::HandleAttackEnd()
{
	if (!CachedOwnerComponent)
	{
		ClearDelegate();
		CachedOwnerCharacter = nullptr;
		return;
	}

	ClearDelegate();
	FinishLatentTask(*CachedOwnerComponent, EBTNodeResult::Succeeded);

	UE_LOG(LogTemp, Warning, TEXT("[%s] AIAttack: 공격 종료"), *CachedOwnerCharacter->GetActorLabel());

	CachedOwnerComponent = nullptr;
	CachedOwnerCharacter = nullptr;
}
