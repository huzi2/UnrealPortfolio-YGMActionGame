// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/BTTask_EnemyMoveTo.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_EnemyMoveTo::UBTTask_EnemyMoveTo()
{
	NodeName = TEXT("Enemy Move To");

	MinAcceptableRadiusKey.SelectedKeyName = TEXT("MinAttackRange");
	MaxAcceptableRadiusKey.SelectedKeyName = TEXT("MaxAttackRange");

	MinAcceptableRadiusKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_EnemyMoveTo, MinAcceptableRadiusKey));
	MaxAcceptableRadiusKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_EnemyMoveTo, MaxAcceptableRadiusKey));
}

EBTNodeResult::Type UBTTask_EnemyMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return EBTNodeResult::Failed;

	const float MinAcceptableRadius = BlackboardComp->GetValueAsFloat(MinAcceptableRadiusKey.SelectedKeyName);
	const float MaxAcceptableRadius = BlackboardComp->GetValueAsFloat(MaxAcceptableRadiusKey.SelectedKeyName);
	AcceptableRadius = FMath::RandRange(MinAcceptableRadius, MaxAcceptableRadius);

	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
