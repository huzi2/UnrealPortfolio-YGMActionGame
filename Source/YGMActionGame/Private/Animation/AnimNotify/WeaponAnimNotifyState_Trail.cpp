// Fill out your copyright notice in the Description page of Project Settings.

#include "Animation/AnimNotify/WeaponAnimNotifyState_Trail.h"
#include "Characters/BaseCharacter.h"

UWeaponAnimNotifyState_Trail::UWeaponAnimNotifyState_Trail()
	: bCheckWeapon(false)
	, bPlayTrail(true)
{
}

void UWeaponAnimNotifyState_Trail::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (bCheckWeapon)
	{
		ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
		if (!Character) return;

		Character->GetWeapon() ? bPlayTrail = true : bPlayTrail = false;
	}

	if (!bPlayTrail) return;

	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

void UWeaponAnimNotifyState_Trail::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	if (!bPlayTrail) return;

	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}
void UWeaponAnimNotifyState_Trail::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!bPlayTrail) return;

	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
