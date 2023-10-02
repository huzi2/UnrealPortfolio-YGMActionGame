// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "WeaponAttackBoxAnimNotifyState.generated.h"

class UAttackBoxComponent;
/**
 * 
 */
UCLASS()
class YGMACTIONGAME_API UWeaponAttackBoxAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
private:
	UWeaponAttackBoxAnimNotifyState();

private:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) final;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) final;

private:
	UPROPERTY(EditAnywhere)
	float DamageModifier;

	UPROPERTY()
	UAttackBoxComponent* AttackBox;
};
