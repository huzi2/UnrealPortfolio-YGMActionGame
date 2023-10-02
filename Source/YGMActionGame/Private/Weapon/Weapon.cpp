// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/Weapon.h"
#include "Characters/Components/AttackBoxComponent.h"

AWeapon::AWeapon()
{
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SetRootComponent(WeaponMesh);

	Tags.Add(TEXT("Weapon"));
}

UAttackBoxComponent* AWeapon::GetAttackBox()
{
	return Cast<UAttackBoxComponent>(GetDefaultSubobjectByName(TEXT("AttackBox")));
}

void AWeapon::Equip(USceneComponent* InParent, const FName& InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	if (WeaponMesh && InParent)
	{
		SetOwner(NewOwner);
		SetInstigator(NewInstigator);

		const FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
		WeaponMesh->AttachToComponent(InParent, TransformRules, InSocketName);
	}
}
