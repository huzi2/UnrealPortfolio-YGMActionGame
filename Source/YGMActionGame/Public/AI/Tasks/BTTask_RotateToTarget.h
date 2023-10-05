// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RotateToTarget.generated.h"

/**
 * 
 */
UCLASS()
class YGMACTIONGAME_API UBTTask_RotateToTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
private:
	UBTTask_RotateToTarget();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) final;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) final;
	virtual FString GetStaticDescription() const final;

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere)
	float AcceptanceAngle;

	UPROPERTY()
	AAIController* AIController;

	UPROPERTY()
	AActor* MainActor;

	UPROPERTY()
	AActor* TargetActor;
};
