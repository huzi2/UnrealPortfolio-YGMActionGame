// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class YGMACTIONGAME_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UAttributeComponent();

public:
	FORCEINLINE float GetAttackDamage() const { return AttackDamage; }
	FORCEINLINE float GetAttackSpeed() const { return AttackSpeed; }

	float GetHealthPercent() const;
	float GetStaminaPercent() const;
	bool IsAlive() const;
	void ReceiveDamage(const float Damage);
	void RecoverHealth(const float Heal);
	bool UseStamina(const float StaminaCost);
	void RecoverStamina(const float Heal);
	void RegenStamina(const float DeltaTime);

private:
	UPROPERTY(EditAnywhere, Category = "Attributes")
	float Health;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float MaxHealth;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float Stamina;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float MaxStamina;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float StaminaRegenRate;

	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float AttackDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float AttackSpeed;
};
