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
	, MaxAttackRange(500.f)
	, MinAttackRange(150.f)
	, bDrawDebugRange(false)
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
		GetCharacterMovement()->RotationRate = FRotator(0.f, 200.f, 0.f);
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

	SetEnemyState(EEnemyState::EES_Idle);
	EnemyAIController->SetAttackRange(MinAttackRange, MaxAttackRange);
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugAISphere();
	MotionWarpingToTarget();
}

void AEnemyCharacter::Die()
{
	Super::Die();

	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(5.f);
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
	if (CharacterAnimInstance && CharacterAnimInstance->Montage_IsPlaying(SpawnMontage)) return;

	EnemyAIController->SetTargetActor(SeenPawn);
	SetEnemyState(EEnemyState::EES_Check);
}

void AEnemyCharacter::CheckNextState()
{
	if (!EnemyAIController) return;

	AActor* TargetActor = EnemyAIController->GetTargetActor();
	// Ÿ���� ������ ��� Idle ����
	if (!TargetActor)
	{
		SetEnemyState(EEnemyState::EES_Idle);
		return;
	}

	const float DistanceToTarget = UYGMActionGameLibrary::GetDistance(GetActorLocation(), TargetActor->GetActorLocation());

	// ���� �ν� �Ÿ� ���̸� Ÿ�� ���� �� Idle
	if (DistanceToTarget > SightRange)
	{
		EnemyAIController->SetTargetActor(nullptr);
		SetEnemyState(EEnemyState::EES_Idle);
		return;
	}

	switch (static_cast<EEnemyState>(EnemyAIController->GetEnemyState()))
	{
	case EEnemyState::EES_Check: // ���� �ν� �Ÿ� �ȿ� �־ ���� �ൿ�� �غ�
		// ���� ���� ���� �ʱ�ȭ
		NextAttackMontage = nullptr;
		// ���� �ִ� ���� ��Ÿ��� ��� �ִٸ�
		if (DistanceToTarget > MaxAttackRange)
		{
			// �ּ� ���� ��Ÿ��� �ִ� ���� ��Ÿ� ������ ������ ������ ����
			SetEnemyState(EEnemyState::EES_Dash);
		}
		// ���� �ִ� ���� ��Ÿ� �ȿ� �ִٸ�
		else
		{
			const EDir DirectionToTarget = UYGMActionGameLibrary::GetDirection(this, TargetActor->GetActorLocation());
			// ���� ���¿��� ����� ���� �ִϸ��̼� Ȯ��
			NextAttackMontage = GetAttackMontage(DistanceToTarget, DirectionToTarget);

			// ������ ������ ���ų� Ȯ���� ������ �������� �ʾ����� �̵�
			if (!NextAttackMontage)
			{
				SetEnemyState(EEnemyState::EES_Dash);
			}
			else
			{
				// �����̸� �ٷ� ����
				if (DirectionToTarget == EDir::ED_Front)
				{
					SetEnemyState(EEnemyState::EES_Attack);
				}
				else
				{
					// ȸ�� Ȥ�� ȸ���� ���Ե� ���� ��Ÿ�� ����. ���� ��Ÿ�ְ� �ִٸ� 50% Ȯ���� �ߵ�
					if (UYGMActionGameLibrary::CheckPercentage(50) && NextAttackMontage)
					{
						SetEnemyState(EEnemyState::EES_Attack);
					}
					else
					{
						SetEnemyState(EEnemyState::EES_Rotate);
					}
				}
			}
		}
		break;
	case EEnemyState::EES_Rotate: // ���� ������ �ƴ϶� ȸ���ؾ���
	case EEnemyState::EES_Dash: // ������ �ٰ�����
	case EEnemyState::EES_Attack: // ��Ÿ��� ������ ��� �����ϸ� ���� ����
		// �� �ൿ���� �����̺�� Ʈ������ ����. ���Ŀ� üũ�� �ٽ� ����
		SetEnemyState(EEnemyState::EES_Check);
		break;
	default:
		break;
	}
}

UAnimMontage* AEnemyCharacter::GetAttackMontage(const float DistanceToTarget, const EDir DirectionToTarget)
{
	if (!EnemyAttackMontages.Contains(DirectionToTarget)) return nullptr;

	for (const FEnemyAttackRangeMontage& EnemyAttackRangeMontage : EnemyAttackMontages[DirectionToTarget].EnemyAttackRangeMontageArray)
	{
		if (EnemyAttackRangeMontage.MinAttackRange <= DistanceToTarget && EnemyAttackRangeMontage.MaxAttackRange >= DistanceToTarget)
		{
			if (UYGMActionGameLibrary::CheckPercentage(EnemyAttackRangeMontage.AttackPercentage))
			{
				return EnemyAttackRangeMontage.AttackMontage;
			}
		}
	}
	return nullptr;
}

void AEnemyCharacter::SetEnemyState(const EEnemyState State)
{
	if (!EnemyAIController) return;
	EnemyAIController->SetEnemyState(static_cast<uint8>(State));
}

void AEnemyCharacter::DrawDebugAISphere()
{
	if (bDrawDebugRange && GetWorld())
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), SightRange, 12, FColor::Blue, false, -1.f);
		DrawDebugSphere(GetWorld(), GetActorLocation(), MaxAttackRange, 12, FColor::Red, false, -1.f);
		DrawDebugSphere(GetWorld(), GetActorLocation(), MinAttackRange, 12, FColor::Purple, false, -1.f);
	}
}

void AEnemyCharacter::MotionWarpingToTarget()
{
	if (!MotionWarpingComponent) return;

	AActor* TargetActor = EnemyAIController->GetTargetActor();
	if (!TargetActor) return;

	MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(TEXT("Target"), TargetActor->GetActorLocation());
}
