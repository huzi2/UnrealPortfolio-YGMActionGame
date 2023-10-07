// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_EnemyMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class YGMACTIONGAME_API UBTTask_EnemyMoveTo : public UBTTask_MoveTo
{
	GENERATED_BODY()
	
private:
	UBTTask_EnemyMoveTo();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector MinAcceptableRadiusKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector MaxAcceptableRadiusKey;
};
