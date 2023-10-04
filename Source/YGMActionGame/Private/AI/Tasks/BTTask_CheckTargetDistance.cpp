// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/BTTask_CheckTargetDistance.h"
#include "AIController.h"
#include "Characters/Enemy/EnemyCharacter.h"

UBTTask_CheckTargetDistance::UBTTask_CheckTargetDistance()
{
	NodeName = TEXT("Check Target Distance");
}

EBTNodeResult::Type UBTTask_CheckTargetDistance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return EBTNodeResult::Failed;

	AEnemyCharacter* Character = Cast<AEnemyCharacter>(Controller->GetPawn());
	if (!Character) return EBTNodeResult::Failed;

	Character->CheckTargetDistance();

	return EBTNodeResult::Succeeded;
}
