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
	FORCEINLINE float GetAttackSpeed() const { return AttackSpeed; }

	float GetHealthPercent() const;
	bool IsAlive() const;
	void ReceiveDamage(const float Damage);

private:
	UPROPERTY(EditAnywhere, Category = "Attributes")
	float Health;

	UPROPERTY(EditAnywhere, Category = "Attributes")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float AttackSpeed;
};
