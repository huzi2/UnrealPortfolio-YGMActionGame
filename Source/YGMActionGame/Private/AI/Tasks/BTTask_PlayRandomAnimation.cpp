// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/BTTask_PlayRandomAnimation.h"
#include "AIController.h"
#include "Characters/BaseCharacter.h"
#include "Animation/CharacterAnimInstance.h"

UBTTask_PlayRandomAnimation::UBTTask_PlayRandomAnimation()
	: bCheckAttackSpeed(false)
{
	NodeName = TEXT("Play Random Animation");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_PlayRandomAnimation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return EBTNodeResult::Failed;

	ABaseCharacter* Character = Cast<ABaseCharacter>(Controller->GetPawn());
	if (!Character) return EBTNodeResult::Failed;

	CharacterAnimInstance = Character->GetCharacterAnimInstance();
	if (!CharacterAnimInstance) return EBTNodeResult::Failed;

	float AnimationSpeed = 1.f;
	if (bCheckAttackSpeed)
	{
		AnimationSpeed = Character->GetAttackSpeed();
	}

	CharacterAnimInstance->PlayRandomMontageSection(Animation, AnimationSpeed);
	return EBTNodeResult::InProgress;
}

void UBTTask_PlayRandomAnimation::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!CharacterAnimInstance) return;

	if (!CharacterAnimInstance->Montage_IsPlaying(Animation))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
