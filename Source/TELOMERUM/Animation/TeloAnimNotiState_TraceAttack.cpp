// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/TeloAnimNotiState_TraceAttack.h"

#include "Components/SkeletalMeshComponent.h"

#include "TeloCharacterBase.h"

void UTeloAnimNotiState_TraceAttack::NotifyBegin(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float TotalDuration,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp) return;

	ATeloCharacterBase* Character = Cast<ATeloCharacterBase>(MeshComp->GetOwner());
	if (!Character) return;

	Character->StartAttackTrace();
}

void UTeloAnimNotiState_TraceAttack::NotifyTick(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float FrameDeltaTime,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (!MeshComp) return;

	ATeloCharacterBase* Character = Cast<ATeloCharacterBase>(MeshComp->GetOwner());
	if (!Character) return;

	Character->TickAttackTrace();
}

void UTeloAnimNotiState_TraceAttack::NotifyEnd(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!MeshComp) return;

	ATeloCharacterBase* Character = Cast<ATeloCharacterBase>(MeshComp->GetOwner());
	if (!Character) return;

	Character->EndAttackTrace();
}