// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/CharacterAnimInstance.h"

void UCharacterAnimInstance::PlayMontage(UAnimMontage* Montage)
{
	if (!Montage) return;

	Montage_Play(Montage);
}

void UCharacterAnimInstance::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	if (!Montage) return;

	Montage_Play(Montage);
	Montage_JumpToSection(SectionName, Montage);
}
