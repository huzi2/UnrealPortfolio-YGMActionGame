// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
	: Health(100.f)
	, MaxHealth(100.f)
	, Stamina(100.f)
	, MaxStamina(100.f)
	, StaminaRegenRate(5.f)
	, AttackDamage(10.f)
	, AttackSpeed(1.3f)
{
}

float UAttributeComponent::GetHealthPercent() const
{
	return Health / MaxHealth;
}

float UAttributeComponent::GetStaminaPercent() const
{
	return Stamina / MaxStamina;
}

bool UAttributeComponent::IsAlive() const
{
	return Health > 0.f;
}

void UAttributeComponent::ReceiveDamage(const float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}

void UAttributeComponent::RecoverHealth(const float Heal)
{
	Health = FMath::Clamp(Health + Heal, 0.f, MaxHealth);
}

bool UAttributeComponent::UseStamina(const float StaminaCost)
{
	if (StaminaCost > Stamina) return false;

	Stamina = FMath::Clamp(Stamina - StaminaCost, 0.f, MaxStamina);
	return true;
}

void UAttributeComponent::RecoverStamina(const float Heal)
{
	Stamina = FMath::Clamp(Stamina + Heal, 0.f, MaxStamina);
}

void UAttributeComponent::RegenStamina(const float DeltaTime)
{
	Stamina = FMath::Clamp(Stamina + StaminaRegenRate * DeltaTime, 0.f, MaxStamina);
}
