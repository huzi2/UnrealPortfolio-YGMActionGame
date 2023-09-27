// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Idle UMETA(DisplayName = "Idle"),
	ECS_SpeedyMove UMETA(DisplayName = "SpeedyMove"),
	ECS_Attack1 UMETA(DisplayName = "Attack1"),
	ECS_Attack2 UMETA(DisplayName = "Attack2"),
	ECS_Attack3 UMETA(DisplayName = "Attack3"),
	ECS_SmashAfterAttack1 UMETA(DisplayName = "SmashAfterAttack1"),
	ECS_Smash0 UMETA(DisplayName = "Smash0"),
	ECS_Smash1a UMETA(DisplayName = "Smash1a"),
	ECS_Smash1b UMETA(DisplayName = "Smash1b"),
	ECS_Smash1c UMETA(DisplayName = "Smash1c"),
	ECS_Smash2a UMETA(DisplayName = "Smash2a"),
	ECS_Smash2b UMETA(DisplayName = "Smash2b"),
	ECS_Smash2c UMETA(DisplayName = "Smash2c"),
	ECS_Smash3a UMETA(DisplayName = "Smash3a"),
	ECS_Smash3b UMETA(DisplayName = "Smash3b"),
	ECS_Smash3c UMETA(DisplayName = "Smash3c"),
	ECS_SpeedyMoveSmash UMETA(DisplayName = "SpeedyMoveSmash"),
};
/**
 * 
 */
UCLASS()
class YGMACTIONGAME_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

private:
	APlayerCharacter(const FObjectInitializer& ObjInit);

private:
	virtual void BeginPlay() final;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) final;

private:
	virtual void PlayDirectionalHitReact(const FVector& ImpactPoint) final;

public:
	bool IsSprinting() const;
	void ResetState();

private:
	// Inputs
	void MoveForward(const FInputActionValue& Value);
	void MoveRight(const FInputActionValue& Value);
	void Turn(const FInputActionValue& Value);
	void LookUp(const FInputActionValue& Value);
	void OnStartSprint();
	void OnStopSprint();
	void SpeedyMove();
	void Attack();
	void Smash();

	void StopAnimation();
	void RotateToController();
	bool CanAttack() const;
	bool CanMove() const;

private:
	// Components
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* CameraComponent;

	// Animations
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* SprintStopMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* SpeedyMoveMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* SmashMontage;

	// Inputs
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* DefaultInputMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveForwardInputAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveRightInputAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* TurnInputAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookUpInputAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SprintInputAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SpeedyMoveInputAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* AttackInputAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SmashInputAction;

private:
	bool bIsSprint;
	ECharacterState CharacterState;
};
