// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "Characters/CharacterTypes.h"
#include "BTDecorator_EnemyState.generated.h"

/**
 * 
 */
UCLASS()
class YGMACTIONGAME_API UBTDecorator_EnemyState : public UBTDecorator
{
	GENERATED_BODY()

private:
	UBTDecorator_EnemyState();

private:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const final;
	virtual FString GetStaticDescription() const final;

private:
	UPROPERTY(EditAnywhere)
	EEnemyState CheckEnemyState;
};
