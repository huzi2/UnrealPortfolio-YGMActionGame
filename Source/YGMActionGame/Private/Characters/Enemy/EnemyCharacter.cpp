// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/EnemyCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/EnemyAIController.h"
#include "Animation/CharacterAnimInstance.h"

AEnemyCharacter::AEnemyCharacter(const FObjectInitializer& ObjInit)
	: Super(ObjInit)
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensingComponent->SightRadius = 2500.f;
	PawnSensingComponent->SetPeripheralVisionAngle(45.f);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	bUseControllerRotationYaw = false;
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 100.f, 0.f);
	}

	AIControllerClass = AEnemyAIController::StaticClass();

	Tags.Add(TEXT("Enemy"));
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	EnemyAIController = Cast<AEnemyAIController>(GetController());

	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemyCharacter::PawnSee);
	}
}

float AEnemyCharacter::GetMovementDirection() const
{
	if (GetVelocity().IsZero()) return 0.f;

	const FVector VelocityNormal = GetVelocity().GetSafeNormal();
	const float AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const FVector CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	const float Degree = FMath::RadiansToDegrees(AngleBetween);
	return CrossProduct.IsZero() ? Degree : Degree * FMath::Sign(CrossProduct.Z);
}

void AEnemyCharacter::PawnSee(APawn* SeenPawn)
{
	if (!SeenPawn) return;
	if (!EnemyAIController) return;

	EnemyAIController->SetTargetActor(SeenPawn);
}
