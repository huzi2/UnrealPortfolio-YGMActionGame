// Fill out your copyright notice in the Description page of Project Settings.

#include "Animation/AnimNotify/WeaponAttackBoxAnimNotifyState.h"
#include "Characters/BaseCharacter.h"
#include "Characters/Components/AttackBoxComponent.h"
#include "Weapon/Weapon.h"

UWeaponAttackBoxAnimNotifyState::UWeaponAttackBoxAnimNotifyState()
	: DamageModifier(1.f)
{
}

void UWeaponAttackBoxAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp) return;

	ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (!Character) return;

	AWeapon* Weapon = Character->GetWeapon();
	if (!Weapon) return;

	AttackBox = Weapon->GetAttackBox();
	if (!AttackBox) return;

	AttackBox->SetDamageModifier(DamageModifier);
	AttackBox->AttackBoxEnable(true);
}

void UWeaponAttackBoxAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!AttackBox) return;

	AttackBox->SetDamageModifier(1.f);
	AttackBox->AttackBoxEnable(false);
}
