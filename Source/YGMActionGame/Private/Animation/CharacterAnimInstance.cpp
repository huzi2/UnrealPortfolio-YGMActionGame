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

	const int32 SectionIndex = Montage->GetSectionIndex(SectionName);
	if (SectionIndex != INDEX_NONE)
	{
		Montage_JumpToSection(SectionName, Montage);
	}
}

void UCharacterAnimInstance::PlayRandomMontageSection(UAnimMontage* Montage, const float PlaySpeed)
{
	if (!Montage) return;

	const TArray<FCompositeSection>& Sections = Montage->CompositeSections;

	const int32 MaxSectionIndex = Sections.Num() - 1;
	const int32 Selection = FMath::RandRange(0, MaxSectionIndex);
	PlayMontageSection(Montage, Sections[Selection].SectionName, PlaySpeed);
}
