// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/TeloAnimNoti_EndAttack.h"

#include "TeloCharacterBase.h"

void UTeloAnimNoti_EndAttack::Notify(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComponent, Animation, EventReference);

	if (!MeshComponent) return;

	AActor* OwnerActor = MeshComponent->GetOwner();
	if (!OwnerActor) return;

	ATeloCharacterBase* Character = Cast<ATeloCharacterBase>(OwnerActor);
	if (!Character) return;

	UE_LOG(LogTemp, Display, TEXT("[%s] AnimNotify End Attack"), *Character->GetActorNameOrLabel());
	Character->EndAttack();
}
