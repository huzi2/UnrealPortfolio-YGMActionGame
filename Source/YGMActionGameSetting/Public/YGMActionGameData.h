// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "YGMActionGameData.generated.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Idle UMETA(DisplayName = "Idle"),
	ECS_SpeedyMove UMETA(DisplayName = "SpeedyMove"),
	ECS_Attack1 UMETA(DisplayName = "Attack1"),
	ECS_Attack2 UMETA(DisplayName = "Attack2"),
	ECS_Attack3 UMETA(DisplayName = "Attack3"),
	ECS_SmashAfterAttack1 UMETA(DisplayName = "SmashAfterAttack1"),
	ECS_Smash0 UMETA(DisplayName = "Smash0"),
	ECS_Smash1a UMETA(DisplayName = "Smash1a"),
	ECS_Smash1b UMETA(DisplayName = "Smash1b"),
	ECS_Smash1c UMETA(DisplayName = "Smash1c"),
	ECS_Smash2a UMETA(DisplayName = "Smash2a"),
	ECS_Smash2b UMETA(DisplayName = "Smash2b"),
	ECS_Smash2c UMETA(DisplayName = "Smash2c"),
	ECS_Smash3a UMETA(DisplayName = "Smash3a"),
	ECS_Smash3b UMETA(DisplayName = "Smash3b"),
	ECS_Smash3c UMETA(DisplayName = "Smash3c"),
	ECS_SpeedyMoveSmash UMETA(DisplayName = "SpeedyMoveSmash"),
};

USTRUCT()
struct FPlayerActionCombo
{
	GENERATED_BODY()

public:
	ECharacterState CurrentCharacterState;
	FString ActionName;
	ECharacterState NextCharacterState;
};
/**
 * 
 */
UCLASS()
class YGMACTIONGAMESETTING_API UYGMActionGameData : public UObject
{
	GENERATED_BODY()
	
private:
	UYGMActionGameData();

public:
	FORCEINLINE const TMap<ECharacterState, std::pair<FName, ECharacterState>>& GetPlayerAttackActionInfo() const { return PlayerAttackActionInfo; }
	FORCEINLINE const TMap<ECharacterState, std::pair<FName, ECharacterState>>& GetPlayerSmashActionInfo() const { return PlayerSmashActionInfo; }

private:
	void ReadActionInfoJsonData(const FString& FilePath, TMap<ECharacterState, std::pair<FName, ECharacterState>>& InfoMap);

private:
	TMap<ECharacterState, std::pair<FName, ECharacterState>> PlayerAttackActionInfo;
	TMap<ECharacterState, std::pair<FName, ECharacterState>> PlayerSmashActionInfo;
};
