// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/EnemyAIController.h"
#include "Characters/Enemy/EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyAIController::AEnemyAIController()
	: TargetActorKeyName(TEXT("TargetActor"))
	, EnemyStateKeyName(TEXT("EnemyState"))
	, MinAttackRagneKeyName(TEXT("MinAttackRange"))
	, MaxAttackRagneKeyName(TEXT("MaxAttackRange"))
{
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(InPawn);
	if (EnemyCharacter)
	{
		RunBehaviorTree(EnemyCharacter->GetBehaviorTree());
	}
}

AActor* AEnemyAIController::GetTargetActor() const
{
	if (!GetBlackboardComponent()) return nullptr;
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(TargetActorKeyName));
}

void AEnemyAIController::SetTargetActor(AActor* Target)
{
	if (!GetBlackboardComponent()) return;
	if (!Target) return;
	if (GetTargetActor() == Target) return;

	if (Target->ActorHasTag(TEXT("Player")))
	{
		SetFocus(Target);
		GetBlackboardComponent()->SetValueAsObject(TargetActorKeyName, Target);
	}
}

uint8 AEnemyAIController::GetEnemyState() const
{
	if (!GetBlackboardComponent()) return 0;
	return GetBlackboardComponent()->GetValueAsEnum(EnemyStateKeyName);
}

void AEnemyAIController::SetEnemyState(const uint8 EnmeyState)
{
	if (!GetBlackboardComponent()) return;
	GetBlackboardComponent()->SetValueAsEnum(EnemyStateKeyName, EnmeyState);
}

void AEnemyAIController::SetAttackRange(const float MinAttackRange, const float MaxAttackRange)
{
	if (!GetBlackboardComponent()) return;
	GetBlackboardComponent()->SetValueAsFloat(MinAttackRagneKeyName, MinAttackRange);
	GetBlackboardComponent()->SetValueAsFloat(MaxAttackRagneKeyName, MaxAttackRange);
}
