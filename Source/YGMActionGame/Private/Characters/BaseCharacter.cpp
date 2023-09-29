// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Characters/Components/AttributeComponent.h"
#include "Characters/Components/AttackBoxComponent.h"
#include "Animation/CharacterAnimInstance.h"
#include "NiagaraFunctionLibrary.h"

ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjInit)
	: Super(ObjInit)
{
	PrimaryActorTick.bCanEverTick = false;

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));
}

void ABaseCharacter::PlayDirectionalHitReact(const FVector& ImpactPoint)
{
	if (!HitReactMontage) return;

	UCharacterAnimInstance* AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (!AnimInstance) return;

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
	FName Section(TEXT("Back"));

	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = TEXT("Front");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = TEXT("Left");
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = TEXT("Right");
	}

	AnimInstance->PlayMontageSection(HitReactMontage, Section);
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
