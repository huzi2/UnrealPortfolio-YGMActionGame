// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UAttackBoxComponent;

UCLASS()
class YGMACTIONGAME_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	ABaseCharacter(const FObjectInitializer& ObjInit);

public:
	UAttackBoxComponent* GetAttackBox(const FName& AttackBoxName);
};
