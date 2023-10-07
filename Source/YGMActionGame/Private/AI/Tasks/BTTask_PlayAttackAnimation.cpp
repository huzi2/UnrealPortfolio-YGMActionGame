// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/BTTask_PlayAttackAnimation.h"
#include "AIController.h"
#include "Characters/Enemy/EnemyCharacter.h"
#include "Animation/CharacterAnimInstance.h"

UBTTask_PlayAttackAnimation::UBTTask_PlayAttackAnimation()
	: bFixAngle(false)
{
	NodeName = TEXT("Play Attack Animation");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_PlayAttackAnimation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return EBTNodeResult::Failed;

	Character = Cast<AEnemyCharacter>(Controller->GetPawn());
	if (!Character) return EBTNodeResult::Failed;

	Animation = Character->GetNextAttackMontage();
	if (!Animation) return EBTNodeResult::Succeeded;

	CharacterAnimInstance = Character->GetCharacterAnimInstance();
	if (!CharacterAnimInstance) return EBTNodeResult::Failed;

	if (bFixAngle)
	{
		Character->FixMeshYawAngle();
	}

	const float AnimationSpeed = Character->GetAttackSpeed();
	CharacterAnimInstance->PlayRandomMontageSection(Animation, AnimationSpeed);

	return EBTNodeResult::InProgress;
}

void UBTTask_PlayAttackAnimation::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!CharacterAnimInstance)
	{
		if (bFixAngle)
		{
			Character->UnFixMeshYawAngle();
		}

		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	if (!CharacterAnimInstance->Montage_IsPlaying(Animation))
	{
		if (bFixAngle)
		{
			Character->UnFixMeshYawAngle();
		}

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
