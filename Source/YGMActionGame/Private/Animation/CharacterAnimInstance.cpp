// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/CharacterAnimInstance.h"

void UCharacterAnimInstance::PlayMontage(UAnimMontage* Montage, const float PlaySpeed)
{
	if (!Montage) return;

	Montage_Play(Montage, PlaySpeed);
}

void UCharacterAnimInstance::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName, const float PlaySpeed)
{
	if (!Montage) return;

	Montage_Play(Montage, PlaySpeed);
	Montage_JumpToSection(SectionName, Montage);
}
