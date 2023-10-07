// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/BTTask_CheckNextState.h"
#include "AIController.h"
#include "Characters/Enemy/EnemyCharacter.h"

UBTTask_CheckNextState::UBTTask_CheckNextState()
{
	NodeName = TEXT("Check Next State");
}

EBTNodeResult::Type UBTTask_CheckNextState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return EBTNodeResult::Failed;

	AEnemyCharacter* Character = Cast<AEnemyCharacter>(Controller->GetPawn());
	if (!Character) return EBTNodeResult::Failed;

	Character->CheckNextState();

	return EBTNodeResult::Succeeded;
}
