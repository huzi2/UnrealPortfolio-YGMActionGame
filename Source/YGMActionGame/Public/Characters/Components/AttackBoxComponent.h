// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "AttackBoxComponent.generated.h"

class ABaseCharacter;
/**
 * 
 */
UCLASS(meta = (DisplayName = "Attack Box", BlueprintSpawnableComponent))
class YGMACTIONGAME_API UAttackBoxComponent : public UBoxComponent
{
	GENERATED_BODY()

private:
	UAttackBoxComponent();

private:
	virtual void BeginPlay() final;

private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	void SetOwnerCharacter();
	bool CheckTarget(AActor* OtherActor);
	void BoxTrace(FHitResult& BoxHit);

public:
	FORCEINLINE void SetDamageModifier(const float Modifier) { DamageModifier = Modifier; }

	void AttackBoxEnable(const bool bEnable);

private:
	UPROPERTY(EditAnywhere, Category = "Attack Box")
	bool bBoxVisible;

	UPROPERTY()
	ABaseCharacter* OwnerCharacter;

	UPROPERTY()
	TArray<AActor*> IgnoreAttackActors;

private:
	float DamageModifier;
};
