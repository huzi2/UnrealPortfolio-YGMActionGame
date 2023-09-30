// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UAttackBoxComponent;
class UAttributeComponent;
class UHealthBarComponent;
class UNiagaraSystem;

UCLASS()
class YGMACTIONGAME_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	ABaseCharacter(const FObjectInitializer& ObjInit);

protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	virtual void PlayDirectionalHitReact(const FVector& ImpactPoint);

public:
	float GetAttackDamage() const;
	float GetAttackSpeed() const;
	UAttackBoxComponent* GetAttackBox(const FName& AttackBoxName);
	void PlayAttackEffect(const FVector& EffectLocation);

protected:
	// Components
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UHealthBarComponent* HealthBarComponent;

private:
	// Animations
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* HitReactMontage;

	// Effects
	UPROPERTY(EditAnywhere, Category = "Effects")
	UNiagaraSystem* AttackEffect;
};
