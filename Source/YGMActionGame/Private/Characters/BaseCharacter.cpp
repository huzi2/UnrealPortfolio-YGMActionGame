// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Characters/Components/AttackBoxComponent.h"

ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjInit)
	: Super(ObjInit)
{
	PrimaryActorTick.bCanEverTick = false;
}

UAttackBoxComponent* ABaseCharacter::GetAttackBox(const FName& AttackBoxName)
{
	return Cast<UAttackBoxComponent>(GetDefaultSubobjectByName(AttackBoxName));
}
