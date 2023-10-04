// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/BTTask_RotateToTarget.h"
#include "AI/EnemyAIController.h"

UBTTask_RotateToTarget::UBTTask_RotateToTarget()
	: AcceptanceAngle(15.f)
{
	NodeName = TEXT("Rotate To Target");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_RotateToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EnemyAIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (!EnemyAIController) return EBTNodeResult::Failed;

	if(!EnemyAIController->SetFocusToTarget()) return EBTNodeResult::Failed;

	return EBTNodeResult::InProgress;
}

void UBTTask_RotateToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const float TargetToAngle = EnemyAIController->GetFacingTargetAngle();
	if (TargetToAngle <= AcceptanceAngle)
	{
		EnemyAIController->ClearFocus(EAIFocusPriority::Gameplay);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
