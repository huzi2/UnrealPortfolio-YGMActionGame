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

	// 들어오는 공격이 어디쪽인지 확인. 전방벡터와 원점->공격지점 벡터끼리 내적
	const FVector Forward = GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// 내적값에 Acos하면 각도가 나온다(|A|*|B| = cos(theta))
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	// 하지만 위 결과로는 양수만 나와서 각도는 알아도 왼쪽인지 오른쪽인지 알수가 없다. 그래서 외적함
	// 언리얼엔진에서 외적은 왼손법칙을 따르며 검지와 중지의 외적이 엄지를 따른다.
	// 그래서 엄지 위치(Z값)가 위인지 아래(0보다 작음)인지 확인해서 왼쪽인지 오른쪽인지 확인
	// 왼손으로 검지를 전방벡터, 중지를 히트포인트로 생각하면 중지가 오른쪽일때 엄지가 위로간다
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	// 엄지(Z값)가 아래(0보다 작음)이므로 방향은 왼쪽
	if (CrossProduct.Z < 0.0)
	{
		Theta *= -1.f;
	}

	// -135 ~ 135도
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
