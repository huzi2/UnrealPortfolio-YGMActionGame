// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Decorators/BTDecorator_EnemyState.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_EnemyState::UBTDecorator_EnemyState()
	: CheckEnemyState(EEnemyState::EES_NoState)
{
	NodeName = TEXT("Enemy State");
}

bool UBTDecorator_EnemyState::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return false;

	EEnemyState State = static_cast<EEnemyState>(BlackboardComp->GetValueAsEnum(TEXT("EnemyState")));

	return CheckEnemyState == State ? true : false;
}

FString UBTDecorator_EnemyState::GetStaticDescription() const
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
	if (!EnumPtr) return FString("Invalid");
	const FString EnumString = EnumPtr->GetDisplayNameTextByIndex(static_cast<int32>(CheckEnemyState)).ToString();
	return FString::Printf(TEXT("%s: %s"), *Super::GetStaticDescription(), *EnumString);
}
