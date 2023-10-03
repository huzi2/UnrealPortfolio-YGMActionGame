// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/PlayerCharacter.h"
#include "Characters/Player/Components/PlayerCharacterMovementComponent.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Characters/Components/AttributeComponent.h"
#include "Animation/PlayerCharacterAnimInstance.h"
#include "UI/GameHUD.h"
#include "UI/PlayerOverlay.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"

APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjInit)
	: Super(ObjInit.SetDefaultSubobjectClass<UPlayerCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
	, bIsSprint(false)
	, CharacterState(ECharacterState::ECS_Idle)
{
	PrimaryActorTick.bCanEverTick = true;

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

	Tags.Add(TEXT("Player"));
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

	PlayerCharacterAnimInstance = Cast<UPlayerCharacterAnimInstance>(CharacterAnimInstance);

	InitializePlayerOverlay();

	ActionGameData = GetDefault<UYGMActionGameData>();
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

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (AttributeComponent)
	{
		AttributeComponent->RegenStamina(DeltaSeconds);

		if (PlayerOverlay)
		{
			PlayerOverlay->SetStaminaBarPercent(AttributeComponent->GetStaminaPercent());
		}
	}
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	SetHUDHealth();
	return Damage;
}

void APlayerCharacter::PlayHitReactAnimation(const FVector& ImpactPoint)
{
	Super::PlayHitReactAnimation(ImpactPoint);

	ResetState();
}

bool APlayerCharacter::IsSprinting() const
{
	return bIsSprint && !GetVelocity().IsZero();
}

void APlayerCharacter::ResetState()
{
	CharacterState = ECharacterState::ECS_Idle;

	if (PlayerCharacterAnimInstance)
	{
		PlayerCharacterAnimInstance->ResetState();
	}
}

void APlayerCharacter::MoveForward(const FInputActionValue& Value)
{
	if (!CanMove()) return;

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
	if (!CanMove()) return;

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
	if (!CanMove()) return;

	bIsSprint = true;
}

void APlayerCharacter::OnStopSprint()
{
	if (!CanMove()) return;

	if (IsSprinting() && SprintStopMontage)
	{
		PlayAnimMontage(SprintStopMontage);
	}

	bIsSprint = false;
}

void APlayerCharacter::SpeedyMove()
{
	if (!CanAttack()) return;
	if (!SpeedyMoveMontage) return;
	if (!PlayerCharacterAnimInstance) return;
	if (!UseStamina(20.f)) return;

	RotateToController();

	PlayerCharacterAnimInstance->PlayMontage(SpeedyMoveMontage);

	CharacterState = ECharacterState::ECS_SpeedyMove;
}

void APlayerCharacter::Attack()
{
	if (!CanAttack()) return;
	if (!AttackMontage) return;
	if (!PlayerCharacterAnimInstance) return;
	if (!ActionGameData) return;

	const TMap<ECharacterState, TPair<FName, ECharacterState>>& AttackInfo_Map = ActionGameData->GetPlayerAttackActionInfo();

	const float AttackSpeed = GetAttackSpeed();

	const TPair<FName, ECharacterState>* AttackInfo = AttackInfo_Map.Find(CharacterState);
	if (!AttackInfo)
	{
		PlayerCharacterAnimInstance->PlayMontageSection(AttackMontage, TEXT("Attack1"), AttackSpeed);
		CharacterState = ECharacterState::ECS_Attack1;
		return;
	}

	RotateToController();

	PlayerCharacterAnimInstance->PlayMontageSection(AttackMontage, AttackInfo->Key, AttackSpeed);
	CharacterState = AttackInfo->Value;
}

void APlayerCharacter::Smash()
{
	if (!CanAttack()) return;
	if (!SmashMontage) return;
	if (!PlayerCharacterAnimInstance) return;
	if (!ActionGameData) return;

	/*static const TMap<ECharacterState, std::pair<FName, ECharacterState>> SmashInfo_Map = {
		{ ECharacterState::ECS_Idle, { "Smash0", ECharacterState::ECS_Smash0 }},

		{ ECharacterState::ECS_Attack1, { "Smash1a", ECharacterState::ECS_Smash1a }},
		{ ECharacterState::ECS_Smash1a, { "Smash1b", ECharacterState::ECS_Smash1b }},
		{ ECharacterState::ECS_Smash1b, { "Smash1c", ECharacterState::ECS_Smash1c }},

		{ ECharacterState::ECS_Attack2, { "Smash2a", ECharacterState::ECS_Smash2a }},
		{ ECharacterState::ECS_Smash2a, { "Smash2b", ECharacterState::ECS_Smash2b }},
		{ ECharacterState::ECS_Smash2b, { "Smash2c", ECharacterState::ECS_Smash2c }},

		{ ECharacterState::ECS_Attack3, { "Smash3a", ECharacterState::ECS_Smash3a }},
		{ ECharacterState::ECS_Smash3a, { "Smash3b", ECharacterState::ECS_Smash3b }},
		{ ECharacterState::ECS_Smash3b, { "Smash3c", ECharacterState::ECS_Smash3c }},

		{ ECharacterState::ECS_SmashAfterAttack1, { "Smash1a", ECharacterState::ECS_Smash1a }},

		{ ECharacterState::ECS_SpeedyMove, { "SpeedyMoveSmash", ECharacterState::ECS_SpeedyMoveSmash }},
	};*/

	const TMap<ECharacterState, TPair<FName, ECharacterState>>& SmashInfo_Map = ActionGameData->GetPlayerSmashActionInfo();

	const TPair<FName, ECharacterState>* SmashInfo = SmashInfo_Map.Find(CharacterState);
	if (!SmashInfo) return;

	if (!UseStamina(10.f)) return;

	RotateToController();

	PlayerCharacterAnimInstance->PlayMontageSection(SmashMontage, SmashInfo->Key, GetAttackSpeed());
	CharacterState = SmashInfo->Value;
}

void APlayerCharacter::InitializePlayerOverlay()
{
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		AGameHUD* GameHUD = Cast<AGameHUD>(PlayerController->GetHUD());
		if (GameHUD)
		{
			PlayerOverlay = GameHUD->GetPlayerOverlay();
			SetHUDHealth();
			SetHUDStamina();
		}
	}
}

void APlayerCharacter::SetHUDHealth()
{
	if (PlayerOverlay && AttributeComponent)
	{
		PlayerOverlay->SetHealthBarPercent(AttributeComponent->GetHealthPercent());
	}
}

void APlayerCharacter::SetHUDStamina()
{
	if (PlayerOverlay && AttributeComponent)
	{
		PlayerOverlay->SetStaminaBarPercent(AttributeComponent->GetStaminaPercent());
	}
}

bool APlayerCharacter::UseStamina(const float StaminaCost)
{
	bool bResult = false;
	if (AttributeComponent)
	{
		bResult = AttributeComponent->UseStamina(StaminaCost);
	}

	if (bResult)
	{
		SetHUDStamina();
	}
	return bResult;
}

void APlayerCharacter::StopAnimation()
{
	StopAnimMontage(GetCurrentMontage());
}

void APlayerCharacter::RotateToController()
{
	const FRotator ControlRotation(0.f, GetControlRotation().Yaw, 0.f);
	SetActorRotation(ControlRotation);
}

bool APlayerCharacter::CanAttack() const
{
	if (!PlayerCharacterAnimInstance) return false;

	return PlayerCharacterAnimInstance->CanAttack();
}

bool APlayerCharacter::CanMove() const
{
	if (!PlayerCharacterAnimInstance) return false;

	return PlayerCharacterAnimInstance->CanMove();
}
