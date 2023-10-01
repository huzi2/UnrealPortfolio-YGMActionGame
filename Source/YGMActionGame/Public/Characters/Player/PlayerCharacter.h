// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "YGMActionGameData.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UPlayerOverlay;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

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
	virtual void Tick(float DeltaSeconds) final;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) final;

private:
	virtual void PlayHitReactAnimation(const FVector& ImpactPoint) final;

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

	void InitializePlayerOverlay();
	void SetHUDHealth();
	void SetHUDStamina();
	bool UseStamina(const float StaminaCost);
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

	UPROPERTY()
	UPlayerOverlay* PlayerOverlay;

	UPROPERTY()
	const UYGMActionGameData* ActionGameData;

private:
	bool bIsSprint;
	ECharacterState CharacterState;
};
