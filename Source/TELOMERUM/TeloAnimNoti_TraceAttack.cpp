// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloAnimNoti_TraceAttack.h"
#include "TeloCharacterBase.h"

void UTeloAnimNoti_TraceAttack::Notify(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComponent, Animation);

	if (!MeshComponent) return;

	AActor* OwnerActor = MeshComponent->GetOwner();
	if (!OwnerActor) return;

	ATeloCharacterBase* Character = Cast<ATeloCharacterBase>(OwnerActor);
	if (!Character) return;

	Character->TraceAttack(Character->GetAttackSocketName());
}
