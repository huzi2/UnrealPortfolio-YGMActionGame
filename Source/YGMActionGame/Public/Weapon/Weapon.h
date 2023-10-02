// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UAttackBoxComponent;

UCLASS()
class YGMACTIONGAME_API AWeapon : public AActor
{
	GENERATED_BODY()
	
private:
	AWeapon();

public:
	UAttackBoxComponent* GetAttackBox();
	void Equip(USceneComponent* InParent, const FName& InSocketName, AActor* NewOwner, APawn* NewInstigator);

private:
	// Components
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* WeaponMesh;
};
