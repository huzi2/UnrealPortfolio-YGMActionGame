// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Decorators/BTDecorator_EnemyState.h"
#include "AIController.h"
#include "Characters/Enemy/EnemyCharacter.h"

UBTDecorator_EnemyState::UBTDecorator_EnemyState()
	: CheckEnemyState(EEnemyState::EES_NoState)
{
	NodeName = TEXT("Enemy State");
}

bool UBTDecorator_EnemyState::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return false;

	AEnemyCharacter* Character = Cast<AEnemyCharacter>(Controller->GetPawn());
	if (!Character) return false;

	return Character->GetEnemyState() == CheckEnemyState ? true : false;
}

FString UBTDecorator_EnemyState::GetStaticDescription() const
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
	if (!EnumPtr) return FString("Invalid");
	const FString EnumString = EnumPtr->GetDisplayNameTextByIndex(static_cast<int32>(CheckEnemyState)).ToString();
	return FString::Printf(TEXT("%s: %s"), *Super::GetStaticDescription(), *EnumString);
}
