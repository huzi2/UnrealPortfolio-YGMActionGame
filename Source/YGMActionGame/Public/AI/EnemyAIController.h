// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class YGMACTIONGAME_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
private:
	AEnemyAIController();

private:
	virtual void OnPossess(APawn* InPawn) final;

public:
	AActor* GetTargetActor() const;
	void SetTargetActor(AActor* Target);

	uint8 GetEnemyState() const;
	void SetEnemyState(const uint8 EnmeyState);

	void SetAttackRange(const float MinAttackRange, const float MaxAttackRange);

private:
	UPROPERTY(EditAnywhere, Category = "AI")
	FName TargetActorKeyName;

	UPROPERTY(EditAnywhere, Category = "AI")
	FName EnemyStateKeyName;

	UPROPERTY(EditAnywhere, Category = "AI")
	FName MinAttackRagneKeyName;

	UPROPERTY(EditAnywhere, Category = "AI")
	FName MaxAttackRagneKeyName;
};
