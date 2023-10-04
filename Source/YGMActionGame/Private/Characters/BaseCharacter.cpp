// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Characters/Components/AttributeComponent.h"
#include "Characters/Components/AttackBoxComponent.h"
#include "Characters/Components/HealthBarComponent.h"
#include "Animation/CharacterAnimInstance.h"
#include "NiagaraFunctionLibrary.h"
#include "Weapon/Weapon.h"

ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjInit)
	: Super(ObjInit)
{
	PrimaryActorTick.bCanEverTick = false;

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));

	HealthBarComponent = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBarComponent"));
	HealthBarComponent->SetupAttachment(GetRootComponent());
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	CharacterAnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (CharacterAnimInstance)
	{
		CharacterAnimInstance->OnMontageEnded.AddDynamic(this, &ABaseCharacter::OnMontageEnded);
	}

	SetWeapon();
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (AttributeComponent)
	{
		AttributeComponent->ReceiveDamage(DamageAmount);

		if (HealthBarComponent)
		{
			HealthBarComponent->SetHealthBarPercent(AttributeComponent->GetHealthPercent());
		}

		if (!AttributeComponent->IsAlive())
		{
			Die();
		}
	}
	return Damage;
}

void ABaseCharacter::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == DieMontage)
	{
		SetRagDoll();
	}
}

void ABaseCharacter::PlayHitReactAnimation(const FVector& ImpactPoint)
{
	if (!HitReactMontage) return;
	if (!CharacterAnimInstance) return;

	const FName Section = GetDirectionalName(ImpactPoint);
	CharacterAnimInstance->PlayMontageSection(HitReactMontage, Section);
}

void ABaseCharacter::PlayDieAnimation(const FVector& ImpactPoint)
{
	if (!DieMontage) return;
	if (!CharacterAnimInstance) return;

	const FName Section = GetDirectionalName(ImpactPoint);
	CharacterAnimInstance->PlayMontageSection(DieMontage, Section);
}

void ABaseCharacter::Die()
{
	HideHealthBar();
	DisableCapsule();
}

bool ABaseCharacter::IsAlive() const
{
	return AttributeComponent ? AttributeComponent->IsAlive() : true;
}

float ABaseCharacter::GetAttackDamage() const
{
	return AttributeComponent ? AttributeComponent->GetAttackDamage() : 10.f;
}

float ABaseCharacter::GetAttackSpeed() const
{
	return AttributeComponent ? AttributeComponent->GetAttackSpeed() : 1.f;
}

UAttackBoxComponent* ABaseCharacter::GetAttackBox(const FName& AttackBoxName)
{
	return Cast<UAttackBoxComponent>(GetDefaultSubobjectByName(AttackBoxName));
}

void ABaseCharacter::PlayAttackEffect(const FVector& EffectLocation)
{
	if (AttackEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, AttackEffect, EffectLocation);
	}
}

const FName ABaseCharacter::GetDirectionalName(const FVector& ImpactPoint)
{
	// ������ ������ ��������� Ȯ��. ���溤�Ϳ� ����->�������� ���ͳ��� ����
	const FVector Forward = GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// �������� Acos�ϸ� ������ ���´�(|A|*|B| = cos(theta))
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	// ������ �� ����δ� ����� ���ͼ� ������ �˾Ƶ� �������� ���������� �˼��� ����. �׷��� ������
	// �𸮾������� ������ �޼չ�Ģ�� ������ ������ ������ ������ ������ ������.
	// �׷��� ���� ��ġ(Z��)�� ������ �Ʒ�(0���� ����)���� Ȯ���ؼ� �������� ���������� Ȯ��
	// �޼����� ������ ���溤��, ������ ��Ʈ����Ʈ�� �����ϸ� ������ �������϶� ������ ���ΰ���
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	// ����(Z��)�� �Ʒ�(0���� ����)�̹Ƿ� ������ ����
	if (CrossProduct.Z < 0.0)
	{
		Theta *= -1.f;
	}

	// -135 ~ 135��
	FName DirName(TEXT("Back"));

	if (Theta >= -45.f && Theta < 45.f)
	{
		DirName = TEXT("Front");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		DirName = TEXT("Left");
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		DirName = TEXT("Right");
	}
	return DirName;
}

void ABaseCharacter::HideHealthBar()
{
	if (HealthBarComponent)
	{
		HealthBarComponent->SetVisibility(false);
	}
}

void ABaseCharacter::DisableCapsule()
{
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ABaseCharacter::SetRagDoll()
{
	if (GetMesh())
	{
		GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector);
		GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
		GetMesh()->SetSimulatePhysics(true);
	}
}

void ABaseCharacter::SetWeapon()
{
	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		Weapon = World->SpawnActor<AWeapon>(WeaponClass);
		if (Weapon)
		{
			Weapon->Equip(GetMesh(), SocketName, this, this);
		}
	}
}
