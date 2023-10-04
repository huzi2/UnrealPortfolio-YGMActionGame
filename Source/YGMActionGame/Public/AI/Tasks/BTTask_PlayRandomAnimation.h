// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PlayRandomAnimation.generated.h"

class UCharacterAnimInstance;
/**
 * 
 */
UCLASS()
class YGMACTIONGAME_API UBTTask_PlayRandomAnimation : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UBTTask_PlayRandomAnimation();
	
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) final;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) final;

private:
	UPROPERTY(EditAnywhere)
	UAnimMontage* Animation;

	UPROPERTY(EditAnywhere)
	bool bCheckAttackSpeed;

	UPROPERTY()
	UCharacterAnimInstance* CharacterAnimInstance;
};
