// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Enemy/EnemyCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "MotionWarpingComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/EnemyAIController.h"
#include "Animation/CharacterAnimInstance.h"
#include "YGMActionGameLibrary.h"

#include "DrawDebugHelpers.h"

AEnemyCharacter::AEnemyCharacter(const FObjectInitializer& ObjInit)
	: Super(ObjInit)
	, SightRange(2500.f)
	, CheckRange(800.f)
	, MaxAttackRange(500.f)
	, MinAttackRange(150.f)
	, bDrawDebugRange(false)
	, EnemyState(EEnemyState::EES_Idle)
	, AttackDirection(EDir::ED_Front)
{
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensingComponent->SetPeripheralVisionAngle(45.f);
	PawnSensingComponent->SightRadius = SightRange;

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	bUseControllerRotationYaw = false;
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 800.f, 0.f);
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

	DrawDebugAISphere();
	MotionWarpingToTarget();
}

float AEnemyCharacter::GetMovementDirection() const
{
	return UYGMActionGameLibrary::GetMovementDirection(GetVelocity(), GetActorForwardVector());
}

void AEnemyCharacter::PawnSee(APawn* SeenPawn)
{
	if (!SeenPawn) return;
	if (!EnemyAIController) return;
	if (EnemyAIController->GetTargetActor() == SeenPawn) return;

	EnemyAIController->SetTargetActor(SeenPawn);
	EnemyState = EEnemyState::EES_Check;
}

void AEnemyCharacter::CheckTargetDistance()
{
	if (!EnemyAIController) return;

	const float DistanceToTarget = GetTargetDistance();
	if (DistanceToTarget > SightRange)
	{
		EnemyAIController->SetTargetActor(nullptr);
		EnemyState = EEnemyState::EES_Idle;
	}
	else if (DistanceToTarget > CheckRange && DistanceToTarget <= SightRange)
	{
		EnemyState = EEnemyState::EES_Dash;
	}
	else if (DistanceToTarget > MaxAttackRange && DistanceToTarget <= CheckRange)
	{
		bool bDoCheck = FMath::RandBool();
		bDoCheck ? EnemyState = EEnemyState::EES_Check : EnemyState = EEnemyState::EES_Dash;
	}
	else if (DistanceToTarget > MinAttackRange && DistanceToTarget <= MaxAttackRange)
	{
		bool bDoCheck = UYGMActionGameLibrary::CheckPercentage(30);
		bDoCheck ? EnemyState = EEnemyState::EES_Check : EnemyState = EEnemyState::EES_Attack;
	}
	else
	{
		EnemyState = EEnemyState::EES_Attack;
	}

	if (EnemyState == EEnemyState::EES_Attack)
	{
		SetAttackDirection();
	}
}

UAnimMontage* AEnemyCharacter::GetAttackRangeMontage() const
{
	if (AttackDirection == EDir::ED_Front)
	{
		const float DistanceToTarget = GetTargetDistance();

		for (const FEnemyAttackRangeMontage& EnemyAttackRangeMontage : EnemyAttackRangeMontages)
		{
			if (DistanceToTarget >= EnemyAttackRangeMontage.MinAttackRange && DistanceToTarget <= EnemyAttackRangeMontage.MaxAttackRange)
			{
				return EnemyAttackRangeMontage.AttackMontage;
			}
		}
	}
	else
	{
		return DirectionAttackMontages[AttackDirection];
	}
	return nullptr;
}

void AEnemyCharacter::CheckNextState()
{
	if (!EnemyAIController) return;

	AActor* TargetActor = EnemyAIController->GetTargetActor();
	// Ÿ���� ������ ��� Idle ����
	if (!TargetActor)
	{
		EnemyState = EEnemyState::EES_Idle;
		return;
	}

	const float DistanceToTarget = UYGMActionGameLibrary::GetDistance(TargetActor->GetActorLocation(), GetActorLocation());

	// ���� �ν� �Ÿ� ���̸� Ÿ�� ���� �� Idle
	if (DistanceToTarget > SightRange)
	{
		EnemyAIController->SetTargetActor(nullptr);
		EnemyState = EEnemyState::EES_Idle;
		return;
	}

	const EDir DirectionToTarget = UYGMActionGameLibrary::GetDirection(this, TargetActor->GetActorLocation());

	switch (EnemyState)
	{
	case EEnemyState::EES_Check: // ���� �ν� �Ÿ� �ȿ� �־ ���� �ൿ�� �غ�
		// ���� �ִ� ���� ��Ÿ��� ��� �ִٸ�
		if (DistanceToTarget > MaxAttackRange)
		{
			// �ּ� ���� ��Ÿ��� �ִ� ���� ��Ÿ� ������ ������ ������ ����
			EnemyState = EEnemyState::EES_Dash;
		}
		// ���� �ִ� ���� ��Ÿ� �ȿ� �ִٸ�
		else
		{
			// �����̸� �ٷ� ����
			if (DirectionToTarget == EDir::ED_Front)
			{
				EnemyState = EEnemyState::EES_Attack;
			}
			else
			{
				// ȸ�� Ȥ�� ȸ���� ���Ե� ���� ��Ÿ�� ����. ���� ��Ÿ�ְ� �ִٸ� 50% Ȯ���� �ߵ�
				NextAttackMontage = GetAttackMontage(DistanceToTarget, DirectionToTarget);
				if (UYGMActionGameLibrary::CheckPercentage(50) && NextAttackMontage)
				{
					EnemyState = EEnemyState::EES_Attack;
				}
				else
				{
					NextAttackMontage = nullptr;
					EnemyState = EEnemyState::EES_Rotate;
				}
			}
		}
		break;
	case EEnemyState::EES_Rotate: // ���� ������ �ƴ϶� ȸ���ؾ���
		// ȸ�� �Ŀ� �ٽ� Ȯ��
		EnemyState = EEnemyState::EES_Check;
		break;
	case EEnemyState::EES_Dash: // ���� ������ �ƴ϶� ȸ���ؾ���
		// ���� �Ŀ� �ٽ� Ȯ��
		EnemyState = EEnemyState::EES_Check;
		break;
	case EEnemyState::EES_Attack: // ��Ÿ��� ������ ��� �����ϸ� ���� ����
		NextAttackMontage = nullptr;
		// ���� �� �ٽ� Ȯ��
		EnemyState = EEnemyState::EES_Check;
		break;
	default:
		break;
	}
}

UAnimMontage* AEnemyCharacter::GetAttackMontage(const float DistanceToTarget, const EDir DirectionToTarget)
{
	// ���� ������ ������ ���� ����
	if (NextAttackMontage) return NextAttackMontage;

	for (const FEnemyAttackRangeMontage& EnemyAttackRangeMontage : EnemyAttackMontages[DirectionToTarget].EnemyAttackRangeMontageArray)
	{
		if (EnemyAttackRangeMontage.MinAttackRange <= DistanceToTarget && EnemyAttackRangeMontage.MaxAttackRange <= DistanceToTarget)
		{
			return EnemyAttackRangeMontage.AttackMontage;
		}
	}
	return nullptr;
}

void AEnemyCharacter::DrawDebugAISphere()
{
	if (bDrawDebugRange && GetWorld())
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), SightRange, 12, FColor::Blue, false, -1.f);
		DrawDebugSphere(GetWorld(), GetActorLocation(), CheckRange, 12, FColor::Orange, false, -1.f);
		DrawDebugSphere(GetWorld(), GetActorLocation(), MaxAttackRange, 12, FColor::Red, false, -1.f);
		DrawDebugSphere(GetWorld(), GetActorLocation(), MinAttackRange, 12, FColor::Purple, false, -1.f);
	}
}

float AEnemyCharacter::GetTargetDistance() const
{
	if (!EnemyAIController) return 0.f;

	AActor* TargetActor = EnemyAIController->GetTargetActor();
	if (!TargetActor) return 0.f;

	return UYGMActionGameLibrary::GetDistance(TargetActor->GetActorLocation(), GetActorLocation());
}

void AEnemyCharacter::SetAttackDirection()
{
	AActor* TargetActor = EnemyAIController->GetTargetActor();
	if (!TargetActor) return;

	const float DistanceToTarget = GetTargetDistance();
	if (DistanceToTarget <= MinAttackRange)
	{
		AttackDirection = UYGMActionGameLibrary::GetDirection(this, TargetActor->GetActorLocation());
		if (AttackDirection != EDir::ED_Front && DirectionAttackMontages[AttackDirection]) return;
	}
	AttackDirection = EDir::ED_Front;
}

void AEnemyCharacter::MotionWarpingToTarget()
{
	if (!MotionWarpingComponent) return;

	AActor* TargetActor = EnemyAIController->GetTargetActor();
	if (!TargetActor) return;

	MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(TEXT("Target"), TargetActor->GetActorLocation());
}
