// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
/**
 * 
 */
UCLASS()
class YGMACTIONGAME_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
private:
	APlayerCharacter();

private:
	virtual void BeginPlay() final;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) final;

private:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;
};
