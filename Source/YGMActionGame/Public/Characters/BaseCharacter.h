// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UAttackBoxComponent;
class UAttributeComponent;
class UNiagaraSystem;

UCLASS()
class YGMACTIONGAME_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	ABaseCharacter(const FObjectInitializer& ObjInit);

public:
	virtual void PlayDirectionalHitReact(const FVector& ImpactPoint);

public:
	float GetAttackSpeed() const;
	UAttackBoxComponent* GetAttackBox(const FName& AttackBoxName);
	void PlayAttackEffect(const FVector& EffectLocation);

protected:
	// Components
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAttributeComponent* AttributeComponent;

private:
	// Animations
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* HitReactMontage;

	// Effects
	UPROPERTY(EditAnywhere, Category = "Effects")
	UNiagaraSystem* AttackEffect;
};
