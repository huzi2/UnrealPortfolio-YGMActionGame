// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/EnemyAIController.h"
#include "Characters/Enemy/EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyAIController::AEnemyAIController()
	: FocusOnKeyName(TEXT("TargetActor"))
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

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyAIController::SetTargetActor(AActor* Target)
{
	if (!Target) return;
	if (Target == TargetActor) return;

	if (Target->ActorHasTag(TEXT("Player")))
	{
		TargetActor = Target;
		SetFocus(TargetActor);
		GetBlackboardComponent()->SetValueAsObject(FocusOnKeyName, TargetActor);
	}
}

AActor* AEnemyAIController::GetFocusOnActor() const
{
	if (!GetBlackboardComponent()) return nullptr;
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}
