// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/BTTask_RotateToTarget.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "YGMActionGameLibrary.h"

UBTTask_RotateToTarget::UBTTask_RotateToTarget()
	: AcceptanceAngle(15.f)
{
	NodeName = TEXT("Rotate To Target");
	bNotifyTick = true;

	TargetActorKey.SelectedKeyName = TEXT("TargetActor");

	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_RotateToTarget, TargetActorKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTTask_RotateToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return EBTNodeResult::Failed;

	TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!TargetActor) return EBTNodeResult::Failed;

	MainActor = AIController->GetPawn();
	if (!MainActor) return EBTNodeResult::Failed;

	AIController->SetFocus(TargetActor);

	return EBTNodeResult::InProgress;
}

void UBTTask_RotateToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!AIController || !MainActor || !TargetActor)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	const float TargetToAngle = UYGMActionGameLibrary::GetAngleActorToActor(MainActor, TargetActor);
	if (TargetToAngle <= AcceptanceAngle)
	{
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

FString UBTTask_RotateToTarget::GetStaticDescription() const
{
	FString KeyDesc("invalid");
	if (TargetActorKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		KeyDesc = TargetActorKey.SelectedKeyName.ToString();
	}
	return FString::Printf(TEXT("%s: %s"), *Super::GetStaticDescription(), *KeyDesc);
}
