// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Characters/Components/AttributeComponent.h"
#include "Characters/Components/AttackBoxComponent.h"
#include "Characters/Components/HealthBarComponent.h"
#include "Animation/CharacterAnimInstance.h"
#include "NiagaraFunctionLibrary.h"
#include "Weapon/Weapon.h"
#include "YGMActionGameLibrary.h"

ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjInit)
	: Super(ObjInit)
	, FixMeshYawAngleValue(0.f)
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

void ABaseCharacter::FixMeshYawAngle()
{
	if (GetMesh())
	{
		FRotator CurrentRotation = GetMesh()->GetComponentRotation();
		CurrentRotation.Yaw += FixMeshYawAngleValue;
		GetMesh()->SetWorldRotation(CurrentRotation);
	}
}

void ABaseCharacter::UnFixMeshYawAngle()
{
	if (GetMesh())
	{
		FRotator CurrentRotation = GetMesh()->GetComponentRotation();
		CurrentRotation.Yaw -= FixMeshYawAngleValue;
		GetMesh()->SetWorldRotation(CurrentRotation);
	}
}

const FName ABaseCharacter::GetDirectionalName(const FVector& ImpactPoint)
{
	const EDir Direction = UYGMActionGameLibrary::GetDirection(this, ImpactPoint);
	switch (Direction)
	{
	case EDir::ED_Front:
		return TEXT("Front");
	case EDir::ED_Left:
		return TEXT("Left");
	case EDir::ED_Right:
		return TEXT("Right");
	case EDir::ED_Back:
		return TEXT("Back");
	default:
		return TEXT("");
	}
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
