// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PlayAttackAnimation.generated.h"

class AEnemyCharacter;
class UCharacterAnimInstance;
/**
 * 
 */
UCLASS()
class YGMACTIONGAME_API UBTTask_PlayAttackAnimation : public UBTTaskNode
{
	GENERATED_BODY()
	
private:
	UBTTask_PlayAttackAnimation();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) final;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) final;

private:
	UPROPERTY(EditAnywhere)
	bool bFixAngle;

	UPROPERTY()
	UAnimMontage* Animation;

	UPROPERTY()
	AEnemyCharacter* Character;

	UPROPERTY()
	UCharacterAnimInstance* CharacterAnimInstance;
};
