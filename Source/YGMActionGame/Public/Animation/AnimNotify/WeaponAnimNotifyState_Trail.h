// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState_Trail.h"
#include "WeaponAnimNotifyState_Trail.generated.h"

/**
 * 
 */
UCLASS()
class YGMACTIONGAME_API UWeaponAnimNotifyState_Trail : public UAnimNotifyState_Trail
{
	GENERATED_BODY()
	
private:
	UWeaponAnimNotifyState_Trail();

private:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) final;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) final;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) final;

private:
	UPROPERTY(EditAnywhere)
	bool bCheckWeapon;

private:
	bool bPlayTrail;
};
