// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/AttackBoxAnimNotifyState.h"
#include "Characters/BaseCharacter.h"
#include "Characters/Components/AttackBoxComponent.h"
#include "Weapon/Weapon.h"

UAttackBoxAnimNotifyState::UAttackBoxAnimNotifyState()
	: DamageModifier(1.f)
	, bUseWeaponAttackBox(false)
	, WeaponDamageModifier(1.f)
{
}

void UAttackBoxAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp) return;

	ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (!Character) return;

	AttackBox = Character->GetAttackBox(AttackBoxName);
	if (AttackBox)
	{
		AttackBox->SetDamageModifier(DamageModifier);
		AttackBox->AttackBoxEnable(true);
	}

	if (bUseWeaponAttackBox)
	{
		AWeapon* Weapon = Character->GetWeapon();
		if (!Weapon) return;

		WeaponAttackBox = Weapon->GetAttackBox();
		if (WeaponAttackBox)
		{
			WeaponAttackBox->SetDamageModifier(WeaponDamageModifier);
			WeaponAttackBox->AttackBoxEnable(true);
		}
	}
}

void UAttackBoxAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (AttackBox)
	{
		AttackBox->SetDamageModifier(1.f);
		AttackBox->AttackBoxEnable(false);
	}

	if (WeaponAttackBox)
	{
		WeaponAttackBox->SetDamageModifier(1.f);
		WeaponAttackBox->AttackBoxEnable(false);
	}
}
