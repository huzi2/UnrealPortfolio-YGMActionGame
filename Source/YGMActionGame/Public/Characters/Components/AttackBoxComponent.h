// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "AttackBoxComponent.generated.h"

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
	void BoxTrace(FHitResult& BoxHit);

public:
	void AttackBoxEnable(const bool bEnable);
};
