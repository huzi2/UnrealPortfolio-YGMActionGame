// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Characters/CharacterTypes.h"
#include "BTTask_ChangeEnemyState.generated.h"

USTRUCT()
struct FEnemyStatePercentage
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
	EEnemyState EnemyState;

	UPROPERTY(EditAnywhere)
	int32 Percentage;
};

/**
 * 
 */
UCLASS()
class YGMACTIONGAME_API UBTTask_ChangeEnemyState : public UBTTaskNode
{
	GENERATED_BODY()
	
private:
	UBTTask_ChangeEnemyState();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) final;

private:
	EEnemyState GetPercentageEnemyState() const;

private:
	UPROPERTY(EditAnywhere)
	TArray<FEnemyStatePercentage> TargetEnemyStates;
};
