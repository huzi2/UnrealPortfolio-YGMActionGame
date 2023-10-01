// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/AttackBoxComponent.h"
#include "Characters/BaseCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

UAttackBoxComponent::UAttackBoxComponent()
	: bBoxVisible(false)
{
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	bHiddenInGame = false;

	SetLineThickness(5.f);
	ShapeColor = FColor::Red;
}

void UAttackBoxComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UAttackBoxComponent::OnOverlapBegin);

	SetVisibility(false);
}

void UAttackBoxComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner()) return;

	FHitResult BoxHit;
	BoxTrace(BoxHit);

	if (BoxHit.GetActor())
	{
		ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
		if (!OwnerCharacter) return;

		//const FVector& HitPoint = BoxHit.ImpactPoint;
		const FVector& HitPoint = GetComponentLocation();

		OwnerCharacter->PlayAttackEffect(BoxHit.ImpactPoint);

		UGameplayStatics::ApplyDamage(BoxHit.GetActor(), OwnerCharacter->GetAttackDamage() * DamageModifier, OwnerCharacter->GetController(), OwnerCharacter, UDamageType::StaticClass());

		ABaseCharacter* Character = Cast<ABaseCharacter>(BoxHit.GetActor());
		if (!Character) return;

		if (Character->IsAlive())
		{
			Character->PlayHitReactAnimation(HitPoint);
		}
		else
		{
			Character->PlayDieAnimation(HitPoint);
		}
	}
}

void UAttackBoxComponent::BoxTrace(FHitResult& BoxHit)
{
	const FVector& BoxOrigin = GetComponentLocation();
	const FRotator& BoxRotation = GetComponentRotation();

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());

	const bool bHit = UKismetSystemLibrary::BoxTraceSingleForObjects(this, BoxOrigin, BoxOrigin + BoxRotation.Vector() * 1.1f, BoxExtent * 2.f, BoxRotation, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, BoxHit, true);
}

void UAttackBoxComponent::AttackBoxEnable(const bool bEnable)
{
	bEnable ? SetCollisionEnabled(ECollisionEnabled::QueryOnly) : SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (bBoxVisible)
	{
		SetVisibility(bEnable);
	}
}
