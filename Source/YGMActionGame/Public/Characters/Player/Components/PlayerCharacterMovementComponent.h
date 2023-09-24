// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class YGMACTIONGAME_API UPlayerCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
private:
	UPlayerCharacterMovementComponent();

private:
	virtual float GetMaxSpeed() const final;

private:
	UPROPERTY(EditDefaultsOnly)
	float SprintModifier;
};
