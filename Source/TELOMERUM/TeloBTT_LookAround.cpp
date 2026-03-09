#include "TeloBTT_LookAround.h"
#include "AIController.h"
#include "Math/UnrealMathUtility.h"

UTeloBTT_LookAround::UTeloBTT_LookAround()
{
	NodeName = TEXT("Look Around");
	bNotifyTick = true;
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UTeloBTT_LookAround::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AI = OwnerComp.GetAIOwner();
	if (!AI)
	{
		return EBTNodeResult::Failed;
	}

	StartYaw = NormalizeYaw(AI->GetControlRotation().Yaw);
	Phase = 0;

	// 첫 단계: 왼쪽 보기
	TargetYaw = NormalizeYaw(StartYaw - LookAngle);

	return EBTNodeResult::InProgress;
}

void UTeloBTT_LookAround::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UE_LOG(LogTemp, Warning, TEXT("LookAround Tick"));

	AAIController* AI = OwnerComp.GetAIOwner();
	if (!AI)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	FRotator ControlRot = AI->GetControlRotation();
	float CurrentYaw = NormalizeYaw(ControlRot.Yaw);

	float DeltaYaw = FMath::FindDeltaAngleDegrees(CurrentYaw, TargetYaw);
	float MaxStep = RotationSpeed * DeltaSeconds;

	float Step = FMath::Clamp(DeltaYaw, -MaxStep, MaxStep);
	float NewYaw = CurrentYaw + Step;
	NewYaw = FRotator::NormalizeAxis(NewYaw);

	UE_LOG(LogTemp, Warning,
		TEXT("Yaw: Current=%.2f Target=%.2f New=%.2f Phase=%d"),
		CurrentYaw,
		TargetYaw,
		NewYaw,
		Phase
	);

	ControlRot.Yaw = NewYaw;
	AI->SetControlRotation(ControlRot);

	UE_LOG(LogTemp, Warning,
		TEXT("After SetControlRotation: ControllerYaw=%.2f PawnYaw=%.2f"),
		AI->GetControlRotation().Yaw,
		AI->GetPawn() ? AI->GetPawn()->GetActorRotation().Yaw : -999.f
	);

	bool bReached = IsYawReached(NewYaw, TargetYaw);
	
	UE_LOG(LogTemp, Warning,
		TEXT("Reached? %s"),
		bReached ? TEXT("YES") : TEXT("NO")
	);

	if (!bReached)
	{
		return;
	}

	if (Phase == 0)
	{
		// 왼쪽 도달 후 오른쪽으로 전환
		Phase = 1;
		TargetYaw = NormalizeYaw(StartYaw + LookAngle);
		return;
	}

	if (Phase == 1)
	{
		// 오른쪽 도달 시 종료
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

float UTeloBTT_LookAround::NormalizeYaw(float Yaw) const
{
	return FRotator::NormalizeAxis(Yaw);
}

bool UTeloBTT_LookAround::IsYawReached(float CurrentYaw, float InTargetYaw) const
{
	return FMath::Abs(FRotator::NormalizeAxis(CurrentYaw - InTargetYaw)) <= 1.0f;
}