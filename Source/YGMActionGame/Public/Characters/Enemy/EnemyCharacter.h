// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "EnemyCharacter.generated.h"

class UPawnSensingComponent;
class UBehaviorTree;
class AEnemyAIController;
/**
 * 
 */
UCLASS()
class YGMACTIONGAME_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
private:
	AEnemyCharacter(const FObjectInitializer& ObjInit);

private:
	virtual void BeginPlay() final;

protected:
	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetMovementDirection() const;

private:
	UFUNCTION()
	void PawnSee(APawn* SeenPawn);

public:
	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }

private:
	// Components
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComponent;
	
	// AI
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY()
	AEnemyAIController* EnemyAIController;
};
