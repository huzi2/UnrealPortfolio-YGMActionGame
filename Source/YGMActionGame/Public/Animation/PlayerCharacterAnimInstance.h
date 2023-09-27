// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/CharacterAnimInstance.h"
#include "PlayerCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class YGMACTIONGAME_API UPlayerCharacterAnimInstance : public UCharacterAnimInstance
{
	GENERATED_BODY()

private:
	UPlayerCharacterAnimInstance();

private:
	UFUNCTION()
	void AnimNotify_ResetState();

	UFUNCTION()
	void AnimNotify_CanAttack();

	UFUNCTION()
	void AnimNotify_CantAttack();

	UFUNCTION()
	void AnimNotify_CanMove();

public:
	virtual void PlayMontage(UAnimMontage* Montage, const float PlaySpeed = 1.f) final;
	virtual void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName, const float PlaySpeed = 1.f) final;

public:
	FORCEINLINE bool CanAttack() const { return bCanAttack; }
	FORCEINLINE bool CanMove() const { return bCanMove; }

	void ResetState();
	void LockState();

private:
	bool bCanAttack;
	bool bCanMove;
};
