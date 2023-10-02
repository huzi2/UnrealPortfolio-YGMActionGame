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
	SetOwnerCharacter();

	if (!OwnerCharacter) return;
	if (!CheckTarget(OtherActor)) return;

	FHitResult BoxHit;
	BoxTrace(BoxHit);

	if (BoxHit.GetActor())
	{
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

void UAttackBoxComponent::SetOwnerCharacter()
{
	if (GetOwner()->ActorHasTag(TEXT("Weapon")))
	{
		OwnerCharacter = Cast<ABaseCharacter>(GetOwner()->GetOwner());
	}
	else
	{
		OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
	}
}

bool UAttackBoxComponent::CheckTarget(AActor* OtherActor)
{
	if (OtherActor == OwnerCharacter) return false;
	if (OtherActor->ActorHasTag(TEXT("Weapon"))) return false;
	return true;
}

void UAttackBoxComponent::BoxTrace(FHitResult& BoxHit)
{
	const FVector& BoxOrigin = GetComponentLocation();
	const FRotator& BoxRotation = GetComponentRotation();

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(GetOwner());
	ActorsToIgnore.AddUnique(OwnerCharacter);

	for (AActor* Actor : IgnoreAttackActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	if (UKismetSystemLibrary::BoxTraceSingleForObjects(this, BoxOrigin, BoxOrigin + BoxRotation.Vector() * 1.1f, BoxExtent * 2.f, BoxRotation, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, BoxHit, true))
	{
		// 중복 타겟 방지
		IgnoreAttackActors.AddUnique(BoxHit.GetActor());
	}
}

void UAttackBoxComponent::AttackBoxEnable(const bool bEnable)
{
	bEnable ? SetCollisionEnabled(ECollisionEnabled::QueryOnly) : SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (bEnable)
	{
		// 중복 타겟 방지 배열 초기화
		IgnoreAttackActors.Empty();
	}

	if (bBoxVisible)
	{
		SetVisibility(bEnable);
	}
}
