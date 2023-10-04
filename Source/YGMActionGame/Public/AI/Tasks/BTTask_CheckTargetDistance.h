// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CheckTargetDistance.generated.h"

/**
 * 
 */
UCLASS()
class YGMACTIONGAME_API UBTTask_CheckTargetDistance : public UBTTaskNode
{
	GENERATED_BODY()
	
private:
	UBTTask_CheckTargetDistance();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) final;
};
