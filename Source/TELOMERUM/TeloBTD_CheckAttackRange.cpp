// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloBTD_CheckAttackRange.h"
#include "AIController.h"
#include "TeloEnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UTeloBTD_CheckAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComponent, uint8* NodeMemory) const
{
	// AIController 가져오기
	AAIController* AIController = OwnerComponent.GetAIOwner();
	if (!AIController) return false;

	// Pawn 가져오기
	APawn* Pawn = AIController->GetPawn();
	if (!Pawn)
	{
		{
			UE_LOG(LogTemp, Warning, TEXT("나는 이게 어디서 리턴하는지 알아야겟네: 폰 없음"));
			return false;
		}
	}

	// EnemyCharacter로 캐스트
	ATeloEnemyCharacter* Enemy = Cast<ATeloEnemyCharacter>(Pawn);
	if (!Enemy) 
	{
		UE_LOG(LogTemp, Warning, TEXT("나는 이게 어디서 리턴하는지 알아야겟네: 에너미 없음"));
		return false;
	}


	// Blackboard에서 TargetActor 가져오기
	UBlackboardComponent* BB = OwnerComponent.GetBlackboardComponent();
	if (!BB)
	{
		UE_LOG(LogTemp, Warning, TEXT("나는 이게 어디서 리턴하는지 알아야겟네: BB없음"));
		return false;
	}

	AActor* Target = Cast<AActor>(BB->GetValueAsObject(TEXT("TargetActor")));
	if (!Target) 
	{
		UE_LOG(LogTemp, Warning, TEXT("나는 이게 어디서 리턴하는지 알아야겟네: 타겟없음"));
		return false;
	}

	float AttackRange = Enemy->GetCalculatedAttackDistance(); // 이미 계산된 실제 공격 범위
	float Distance = FVector::Dist(Pawn->GetActorLocation(), Target->GetActorLocation());

	UE_LOG(LogTemp, Warning, TEXT("Check Attack Range 리턴값: %s"), (Distance <= AttackRange) ? TEXT("TRUE") : TEXT("FALSE"));
	return Distance <= AttackRange;
}
