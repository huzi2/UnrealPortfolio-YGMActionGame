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
	// 타겟이 없으면 계속 Idle 상태
	if (!TargetActor)
	{
		SetEnemyState(EEnemyState::EES_Idle);
		return;
	}

	const float DistanceToTarget = UYGMActionGameLibrary::GetDistance(GetActorLocation(), TargetActor->GetActorLocation());

	// 적이 인식 거리 밖이면 타겟 해제 후 Idle
	if (DistanceToTarget > SightRange)
	{
		EnemyAIController->SetTargetActor(nullptr);
		SetEnemyState(EEnemyState::EES_Idle);
		return;
	}

	switch (static_cast<EEnemyState>(EnemyAIController->GetEnemyState()))
	{
	case EEnemyState::EES_Check: // 적이 인식 거리 안에 있어서 다음 행동을 준비
		// 이전 공격 정보 초기화
		NextAttackMontage = nullptr;
		// 적이 최대 공격 사거리를 벗어나 있다면
		if (DistanceToTarget > MaxAttackRange)
		{
			// 최소 공격 사거리와 최대 공격 사거리 사이의 범위로 적에게 돌진
			SetEnemyState(EEnemyState::EES_Dash);
		}
		// 적이 최대 공격 사거리 안에 있다면
		else
		{
			const EDir DirectionToTarget = UYGMActionGameLibrary::GetDirection(this, TargetActor->GetActorLocation());
			// 공격 상태에서 사용할 공격 애니메이션 확인
			NextAttackMontage = GetAttackMontage(DistanceToTarget, DirectionToTarget);

			// 설정된 공격이 없거나 확률로 공격이 정해지지 않았으면 이동
			if (!NextAttackMontage)
			{
				SetEnemyState(EEnemyState::EES_Dash);
			}
			else
			{
				// 정면이면 바로 공격
				if (DirectionToTarget == EDir::ED_Front)
				{
					SetEnemyState(EEnemyState::EES_Attack);
				}
				else
				{
					// 회전 혹은 회전이 포함된 공격 몽타주 실행. 공격 몽타주가 있다면 50% 확률로 발동
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
	case EEnemyState::EES_Rotate: // 적이 정면이 아니라 회전해야함
	case EEnemyState::EES_Dash: // 적에게 다가간다
	case EEnemyState::EES_Attack: // 사거리와 방향이 모두 적절하면 공격 수행
		// 위 행동들은 비헤이비어 트리에서 수행. 이후에 체크로 다시 변경
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
