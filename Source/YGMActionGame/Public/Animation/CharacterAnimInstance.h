// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class YGMACTIONGAME_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void PlayMontage(UAnimMontage* Montage);
	virtual void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
};
