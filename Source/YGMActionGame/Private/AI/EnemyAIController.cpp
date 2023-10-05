// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/EnemyAIController.h"
#include "Characters/Enemy/EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyAIController::AEnemyAIController()
	: TargetActorKeyName(TEXT("TargetActor"))
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

AActor* AEnemyAIController::GetTargetActor() const
{
	if (!GetBlackboardComponent()) return nullptr;
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(TargetActorKeyName));
}

void AEnemyAIController::SetTargetActor(AActor* Target)
{
	if (!Target) return;
	if (GetTargetActor() == Target) return;

	if (Target->ActorHasTag(TEXT("Player")))
	{
		SetFocus(Target);
		GetBlackboardComponent()->SetValueAsObject(TargetActorKeyName, Target);
	}
}

float AEnemyAIController::GetFacingTargetAngle() const
{
	if (!GetPawn()) return 0.f;
	
	AActor* TargetActor = GetTargetActor();
	if (!TargetActor) return 0.f;

	const FVector ToTarget = (TargetActor->GetActorLocation() - GetPawn()->GetActorLocation()).GetSafeNormal();
	const FVector ForwardVector = GetPawn()->GetActorForwardVector();
	const float DotProduct = FVector::DotProduct(ToTarget, ForwardVector);
	const float Angle = FMath::Acos(DotProduct) * (180.0f / PI);
	return Angle;
}
