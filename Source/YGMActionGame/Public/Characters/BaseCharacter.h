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
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UFUNCTION()
	virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

public:
	virtual void PlayHitReactAnimation(const FVector& ImpactPoint);
	virtual void PlayDieAnimation(const FVector& ImpactPoint);

private:
	virtual void Die();

public:
	bool IsAlive() const;
	float GetAttackDamage() const;
	float GetAttackSpeed() const;
	UAttackBoxComponent* GetAttackBox(const FName& AttackBoxName);
	void PlayAttackEffect(const FVector& EffectLocation);

private:
	const FName GetDirectionalName(const FVector& ImpactPoint);
	void HideHealthBar();
	void DisableCapsule();
	void SetRagDoll();

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

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* DieMontage;

	// Effects
	UPROPERTY(EditAnywhere, Category = "Effects")
	UNiagaraSystem* AttackEffect;
};
