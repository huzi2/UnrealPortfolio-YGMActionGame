// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy//EnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Animation/CharacterAnimInstance.h"

AEnemyCharacter::AEnemyCharacter(const FObjectInitializer& ObjInit)
	: Super(ObjInit)
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void AEnemyCharacter::PlayRunMontage(const FName& SectionName)
{
	if (!RunMontage) return;

	UCharacterAnimInstance* AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (!AnimInstance) return;

	const FName Section = TEXT("N");
	AnimInstance->PlayMontageSection(RunMontage, Section);
}
