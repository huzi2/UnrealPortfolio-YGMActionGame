// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterTypes.h"
#include "YGMActionGameLibrary.generated.h"

UCLASS()
class YGMACTIONGAME_API UYGMActionGameLibrary : public UObject
{
    GENERATED_BODY()

public:
    static float GetAngleActorToActor(AActor* MainActor, AActor* TargetActor);
    static EDir GetDirection(const AActor* MainActor, const FVector& TargetLocation);
    static bool CheckPercentage(const int32 Percent);
};
