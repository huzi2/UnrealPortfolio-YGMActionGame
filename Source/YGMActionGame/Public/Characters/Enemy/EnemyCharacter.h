// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "EnemyCharacter.generated.h"

class UPawnSensingComponent;
class UMotionWarpingComponent;
class UBehaviorTree;
class AEnemyAIController;

USTRUCT()
struct FEnemyAttackRangeMontage
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere)
	float MinAttackRange;

	UPROPERTY(EditAnywhere)
	float MaxAttackRange;
};

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
	virtual void Tick(float DeltaTime) final;

protected:
	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetMovementDirection() const;

private:
	UFUNCTION()
	void PawnSee(APawn* SeenPawn);

public:
	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }
	FORCEINLINE EEnemyState GetEnemyState() const { return EnemyState; }
	FORCEINLINE void SetEnemyState(const EEnemyState State) { EnemyState = State; }

	void CheckTargetDistance();
	UAnimMontage* GetAttackRangeMontage() const;

private:
	float GetTargetDistance() const;
	void MotionWarpingToTarget();
	
private:
	// Components
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UMotionWarpingComponent* MotionWarpingComponent;

	// Animations
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* SpawnMontage;

	UPROPERTY(EditAnywhere, Category = "Animations")
	TArray<FEnemyAttackRangeMontage> EnemyAttackRangeMontages;

	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* LeftAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* RightAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* BackAttackMontage;
	
	// AI
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, Category = "AI")
	float SightRange;

	UPROPERTY(EditAnywhere, Category = "AI")
	float CheckRange;

	UPROPERTY(EditAnywhere, Category = "AI")
	float MaxAttackRange;

	UPROPERTY(EditAnywhere, Category = "AI")
	float MinAttackRange;

	UPROPERTY(EditAnywhere, Category = "AI")
	bool bDrawDebugRange;

	UPROPERTY()
	AEnemyAIController* EnemyAIController;

private:
	EEnemyState EnemyState;
};
