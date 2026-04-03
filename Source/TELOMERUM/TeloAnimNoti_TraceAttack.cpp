// Fill out your copyright notice in the Description page of Project Settings.


#include "TeloAnimNoti_TraceAttack.h"
#include "TeloCharacterBase.h"

void UTeloAnimNoti_TraceAttack::Notify(USkeletalMeshComponent* MeshComponent, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	UE_LOG(LogTemp, Warning, TEXT(""));
	Super::Notify(MeshComponent, Animation, EventReference);

	if (!MeshComponent) return;

	AActor* OwnerActor = MeshComponent->GetOwner();
	if (!OwnerActor) return;

	ATeloCharacterBase* Character = Cast<ATeloCharacterBase>(OwnerActor);
	if (!Character) return;

	//Character->BeginAttackTrace(Character->GetAttackSocketName());
}
