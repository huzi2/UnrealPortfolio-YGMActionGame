// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AttackBoxAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class YGMACTIONGAME_API UAttackBoxAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
private:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) final;

private:
	UPROPERTY(EditAnywhere)
	FName AttackBoxName;

	UPROPERTY(EditAnywhere)
	bool bEnable;
};
