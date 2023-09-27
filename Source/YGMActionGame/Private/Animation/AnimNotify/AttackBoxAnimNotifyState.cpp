// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/AttackBoxAnimNotifyState.h"
#include "Characters/BaseCharacter.h"
#include "Characters/Components/AttackBoxComponent.h"

void UAttackBoxAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp) return;

	ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (!Character) return;

	AttackBox = Character->GetAttackBox(AttackBoxName);
	if (!AttackBox) return;

	AttackBox->AttackBoxEnable(true);
}

void UAttackBoxAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!AttackBox) return;

	AttackBox->AttackBoxEnable(false);
}
