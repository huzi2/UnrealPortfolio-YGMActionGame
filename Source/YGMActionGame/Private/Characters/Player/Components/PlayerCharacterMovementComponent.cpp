// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/Components/PlayerCharacterMovementComponent.h"
#include "Characters/Player/PlayerCharacter.h"

UPlayerCharacterMovementComponent::UPlayerCharacterMovementComponent()
	: SprintModifier(2.f)
{
}

float UPlayerCharacterMovementComponent::GetMaxSpeed() const
{
	const float MaxSpeed = Super::GetMaxSpeed();
	APlayerCharacter* Player = Cast<APlayerCharacter>(GetPawnOwner());
	return Player && Player->IsSprinting() ? MaxSpeed * SprintModifier : MaxSpeed;
}
