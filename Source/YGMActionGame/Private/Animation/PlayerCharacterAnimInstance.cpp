// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PlayerCharacterAnimInstance.h"
#include "Characters/Player/PlayerCharacter.h"

UPlayerCharacterAnimInstance::UPlayerCharacterAnimInstance()
	: bCanAttack(true)
	, bCanMove(true)
{
}

void UPlayerCharacterAnimInstance::AnimNotify_ResetState()
{
	APawn* Pawn = TryGetPawnOwner();
	if (!IsValid(Pawn)) return;

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Pawn);
	if (PlayerCharacter)
	{
		PlayerCharacter->ResetState();
	}
}

void UPlayerCharacterAnimInstance::AnimNotify_CanAttack()
{
	bCanAttack = true;
}

void UPlayerCharacterAnimInstance::AnimNotify_CantAttack()
{
	bCanAttack = false;
}

void UPlayerCharacterAnimInstance::AnimNotify_CanMove()
{
	bCanMove = true;
}

void UPlayerCharacterAnimInstance::PlayMontage(UAnimMontage* Montage, const float PlaySpeed)
{
	Super::PlayMontage(Montage, PlaySpeed);

	LockState();
}

void UPlayerCharacterAnimInstance::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName, const float PlaySpeed)
{
	Super::PlayMontageSection(Montage, SectionName, PlaySpeed);

	LockState();
}

void UPlayerCharacterAnimInstance::ResetState()
{
	bCanAttack = true;
	bCanMove = true;
}

void UPlayerCharacterAnimInstance::LockState()
{
	bCanAttack = false;
	bCanMove = false;
}
