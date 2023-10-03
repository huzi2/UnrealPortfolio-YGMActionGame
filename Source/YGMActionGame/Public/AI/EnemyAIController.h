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
	virtual void Tick(float DeltaTime) final;

public:
	void SetTargetActor(AActor* Target);

private:
	AActor* GetFocusOnActor() const;

private:
	UPROPERTY(EditAnywhere, Category = "AI")
	FName FocusOnKeyName;

	UPROPERTY()
	AActor* TargetActor;
};
