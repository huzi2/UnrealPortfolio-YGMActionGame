// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AttackBoxAnimNotifyState.generated.h"

class UAttackBoxComponent;
/**
 * 
 */
UCLASS()
class YGMACTIONGAME_API UAttackBoxAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
private:
	UAttackBoxAnimNotifyState();

private:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) final;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) final;

private:
	UPROPERTY(EditAnywhere)
	FName AttackBoxName;

	UPROPERTY(EditAnywhere)
	float DamageModifier;

	UPROPERTY()
	UAttackBoxComponent* AttackBox;
};
