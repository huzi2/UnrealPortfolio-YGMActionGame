// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CheckNextState.generated.h"

/**
 * 
 */
UCLASS()
class YGMACTIONGAME_API UBTTask_CheckNextState : public UBTTaskNode
{
	GENERATED_BODY()
	
private:
	UBTTask_CheckNextState();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) final;
};
