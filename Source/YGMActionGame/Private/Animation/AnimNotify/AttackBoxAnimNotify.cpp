// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/AttackBoxAnimNotify.h"
#include "Characters/BaseCharacter.h"
#include "Characters/Components/AttackBoxComponent.h"

void UAttackBoxAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp) return;

	ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (!Character) return;

	UAttackBoxComponent* AttackBox = Character->GetAttackBox(AttackBoxName);
	if (!AttackBox) return;

	AttackBox->AttackBoxEnable(bEnable);
}
