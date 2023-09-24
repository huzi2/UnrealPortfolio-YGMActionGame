// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/PlayerCharacter.h"
#include "Characters/Player/Components/PlayerCharacterMovementComponent.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"

APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjInit)
	: Super(ObjInit.SetDefaultSubobjectClass<UPlayerCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
	, bIsSprint(false)
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength = 300.f;
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	DefaultInputMappingContext = CreateDefaultSubobject<UInputMappingContext>(TEXT("DefaultInputMappingContext"));
	MoveForwardInputAction = CreateDefaultSubobject<UInputAction>(TEXT("MoveForwardInputAction"));
	MoveRightInputAction = CreateDefaultSubobject<UInputAction>(TEXT("MoveRightInputAction"));
	TurnInputAction = CreateDefaultSubobject<UInputAction>(TEXT("TurnInputAction"));
	LookUpInputAction = CreateDefaultSubobject<UInputAction>(TEXT("LookUpInputAction"));
	SprintInputAction = CreateDefaultSubobject<UInputAction>(TEXT("SprintInputAction"));
	SpeedyMoveInputAction = CreateDefaultSubobject<UInputAction>(TEXT("SpeedyMoveInputAction"));
	AttackInputAction = CreateDefaultSubobject<UInputAction>(TEXT("AttackInputAction"));
	SmashInputAction = CreateDefaultSubobject<UInputAction>(TEXT("SmashInputAction"));
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultInputMappingContext, 0);
		}
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveForwardInputAction, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveForward);
		Input->BindAction(MoveRightInputAction, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveRight);
		Input->BindAction(TurnInputAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Turn);
		Input->BindAction(LookUpInputAction, ETriggerEvent::Triggered, this, &APlayerCharacter::LookUp);
		Input->BindAction(SprintInputAction, ETriggerEvent::Triggered, this, &APlayerCharacter::OnStartSprint);
		Input->BindAction(SprintInputAction, ETriggerEvent::Completed, this, &APlayerCharacter::OnStopSprint);
		Input->BindAction(SpeedyMoveInputAction, ETriggerEvent::Started, this, &APlayerCharacter::SpeedyMove);
		Input->BindAction(AttackInputAction, ETriggerEvent::Started, this, &APlayerCharacter::Attack);
		Input->BindAction(SmashInputAction, ETriggerEvent::Started, this, &APlayerCharacter::Smash);
	}
}

bool APlayerCharacter::IsSprinting() const
{
	return bIsSprint && !GetVelocity().IsZero();
}

void APlayerCharacter::MoveForward(const FInputActionValue& Value)
{
	StopAnimation();

	const float MovementValue = Value.Get<float>();
	if (MovementValue != 0.f)
	{
		const FRotator& ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, MovementValue);
	}
}

void APlayerCharacter::MoveRight(const FInputActionValue& Value)
{
	StopAnimation();

	const float MovementValue = Value.Get<float>();
	if (MovementValue != 0.f)
	{
		const FRotator& ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, MovementValue);
	}
}

void APlayerCharacter::Turn(const FInputActionValue& Value)
{
	const float MovementValue = Value.Get<float>();
	AddControllerYawInput(MovementValue);
}

void APlayerCharacter::LookUp(const FInputActionValue& Value)
{
	const float MovementValue = Value.Get<float>();
	AddControllerPitchInput(MovementValue);
}

void APlayerCharacter::OnStartSprint()
{
	bIsSprint = true;
}

void APlayerCharacter::OnStopSprint()
{
	if (IsSprinting() && SprintStopMontage)
	{
		PlayAnimMontage(SprintStopMontage);
	}

	bIsSprint = false;
}

void APlayerCharacter::SpeedyMove()
{
	const FRotator& ControlRotation = GetControlRotation();
	const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, 0.01f);

	if (SpeedyMoveMontage)
	{
		PlayAnimMontage(SpeedyMoveMontage);
	}

	AddMovementInput(Direction, 0.01f);
}

void APlayerCharacter::Attack()
{
	if (AttackMontage)
	{
		PlayAnimMontage(AttackMontage);
	}
}

void APlayerCharacter::Smash()
{
	if (SmashMontage)
	{
		PlayAnimMontage(SmashMontage);
	}
}

void APlayerCharacter::StopAnimation()
{
	if (GetCurrentMontage() == AttackMontage)
	{
		StopAnimMontage(AttackMontage);
	}

	if (GetCurrentMontage() == SmashMontage)
	{
		StopAnimMontage(SmashMontage);
	}
}
