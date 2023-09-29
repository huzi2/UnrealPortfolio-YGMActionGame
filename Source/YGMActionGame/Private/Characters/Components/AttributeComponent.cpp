// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
	: Health(100.f)
	, MaxHealth(100.f)
	, AttackSpeed(1.3f)
{
}

float UAttributeComponent::GetHealthPercent() const
{
	return Health / MaxHealth;
}

bool UAttributeComponent::IsAlive() const
{
	return Health > 0.f;
}

void UAttributeComponent::ReceiveDamage(const float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}
