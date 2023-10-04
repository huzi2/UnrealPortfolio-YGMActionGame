// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/BTTask_ChangeEnemyState.h"
#include "AIController.h"
#include "Characters/Enemy/EnemyCharacter.h"

UBTTask_ChangeEnemyState::UBTTask_ChangeEnemyState()
{
	NodeName = TEXT("Change Enemy State");
}

EBTNodeResult::Type UBTTask_ChangeEnemyState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return EBTNodeResult::Failed;

	AEnemyCharacter* Character = Cast<AEnemyCharacter>(Controller->GetPawn());
	if (!Character) return EBTNodeResult::Failed;

	const EEnemyState TargetEnemyState = GetPercentageEnemyState();
	Character->SetEnemyState(TargetEnemyState);

	return EBTNodeResult::Succeeded;
}

EEnemyState UBTTask_ChangeEnemyState::GetPercentageEnemyState() const
{
	int32 MaxPercentage = 0;
	for (const FEnemyStatePercentage& EnemyStatePercentage : TargetEnemyStates)
	{
		MaxPercentage += EnemyStatePercentage.Percentage;
	}

	const int32 Selection = FMath::RandRange(1, MaxPercentage);

	EEnemyState TargetEnemyState = EEnemyState::EES_NoState;
	int32 SumPercentage = 0;
	for (const FEnemyStatePercentage& EnemyStatePercentage : TargetEnemyStates)
	{
		SumPercentage += EnemyStatePercentage.Percentage;

		if (Selection <= SumPercentage)
		{
			TargetEnemyState = EnemyStatePercentage.EnemyState;
			break;
		}
	}
	return TargetEnemyState;
}
