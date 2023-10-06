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
	// 타겟이 없으면 계속 Idle 상태
	if (!TargetActor)
	{
		EnemyState = EEnemyState::EES_Idle;
		return;
	}

	const float DistanceToTarget = UYGMActionGameLibrary::GetDistance(TargetActor->GetActorLocation(), GetActorLocation());

	// 적이 인식 거리 밖이면 타겟 해제 후 Idle
	if (DistanceToTarget > SightRange)
	{
		EnemyAIController->SetTargetActor(nullptr);
		EnemyState = EEnemyState::EES_Idle;
		return;
	}

	const EDir DirectionToTarget = UYGMActionGameLibrary::GetDirection(this, TargetActor->GetActorLocation());

	switch (EnemyState)
	{
	case EEnemyState::EES_Check: // 적이 인식 거리 안에 있어서 다음 행동을 준비
		// 적이 최대 공격 사거리를 벗어나 있다면
		if (DistanceToTarget > MaxAttackRange)
		{
			// 최소 공격 사거리와 최대 공격 사거리 사이의 범위로 적에게 돌진
			EnemyState = EEnemyState::EES_Dash;
		}
		// 적이 최대 공격 사거리 안에 있다면
		else
		{
			// 정면이면 바로 공격
			if (DirectionToTarget == EDir::ED_Front)
			{
				EnemyState = EEnemyState::EES_Attack;
			}
			else
			{
				// 회전 혹은 회전이 포함된 공격 몽타주 실행. 공격 몽타주가 있다면 50% 확률로 발동
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
	case EEnemyState::EES_Rotate: // 적이 정면이 아니라 회전해야함
		// 회전 후에 다시 확인
		EnemyState = EEnemyState::EES_Check;
		break;
	case EEnemyState::EES_Dash: // 적이 정면이 아니라 회전해야함
		// 돌진 후에 다시 확인
		EnemyState = EEnemyState::EES_Check;
		break;
	case EEnemyState::EES_Attack: // 사거리와 방향이 모두 적절하면 공격 수행
		NextAttackMontage = nullptr;
		// 공격 후 다시 확인
		EnemyState = EEnemyState::EES_Check;
		break;
	default:
		break;
	}
}

UAnimMontage* AEnemyCharacter::GetAttackMontage(const float DistanceToTarget, const EDir DirectionToTarget)
{
	// 예약 공격이 있으면 먼저 수행
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
