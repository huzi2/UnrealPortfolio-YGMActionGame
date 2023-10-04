// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Enemy/EnemyCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/EnemyAIController.h"
#include "Animation/CharacterAnimInstance.h"

#include "DrawDebugHelpers.h"

AEnemyCharacter::AEnemyCharacter(const FObjectInitializer& ObjInit)
	: Super(ObjInit)
	, SightRange(2500.f)
	, CheckRange(800.f)
	, AttackRange(150.f)
	, bDrawDebugRange(false)
	, EnemyState(EEnemyState::EES_Idle)
{
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensingComponent->SetPeripheralVisionAngle(45.f);
	PawnSensingComponent->SightRadius = SightRange;

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
		PawnSensingComponent->SightRadius = SightRange;
	}

	if (CharacterAnimInstance && SpawnMontage)
	{
		CharacterAnimInstance->PlayMontage(SpawnMontage);
	}
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bDrawDebugRange && GetWorld())
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), SightRange, 12, FColor::Blue, false, -1.f);
		DrawDebugSphere(GetWorld(), GetActorLocation(), CheckRange, 12, FColor::Orange, false, -1.f);
		DrawDebugSphere(GetWorld(), GetActorLocation(), AttackRange, 12, FColor::Red, false, -1.f);
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
	if (EnemyAIController->GetTargetActor()) return;

	EnemyAIController->SetTargetActor(SeenPawn);
	CheckTargetDistance();
}

void AEnemyCharacter::CheckTargetDistance()
{
	if (!EnemyAIController) return;

	AActor* TargetActor = EnemyAIController->GetTargetActor();
	if (!TargetActor) return;

	const float DistanceToTarget = static_cast<float>((TargetActor->GetActorLocation() - GetActorLocation()).Size());
	if (DistanceToTarget > SightRange)
	{
		EnemyAIController->SetTargetActor(nullptr);
		EnemyState = EEnemyState::EES_Idle;
	}
	else if (DistanceToTarget > CheckRange && DistanceToTarget <= SightRange)
	{
		EnemyState = EEnemyState::EES_Dash;
	}
	else if (DistanceToTarget > AttackRange && DistanceToTarget <= CheckRange)
	{
		bool bDoCheck = FMath::RandBool();
		bDoCheck ? EnemyState = EEnemyState::EES_Check : EnemyState = EEnemyState::EES_Dash;
	}
	else
	{
		EnemyState = EEnemyState::EES_Attack;
	}
}

UAnimMontage* AEnemyCharacter::GetAttackRangeMontage(const float DistanceToTarget) const
{
	for (const FEnemyAttackRangeMontage& EnemyAttackRangeMontage : EnemyAttackRangeMontages)
	{
		if (DistanceToTarget >= EnemyAttackRangeMontage.MinAttackRange && DistanceToTarget <= EnemyAttackRangeMontage.MaxAttackRange)
		{
			return EnemyAttackRangeMontage.AttackMontage;
		}
	}
	return nullptr;
}
