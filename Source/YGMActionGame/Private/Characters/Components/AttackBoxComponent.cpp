// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Components/AttackBoxComponent.h"

UAttackBoxComponent::UAttackBoxComponent()
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

	UE_LOG(LogTemp, Warning, TEXT("Hit!!!!"));
}

void UAttackBoxComponent::AttackBoxEnable(const bool bEnable)
{
	bEnable ? SetCollisionEnabled(ECollisionEnabled::QueryOnly) : SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetVisibility(bEnable);
}
