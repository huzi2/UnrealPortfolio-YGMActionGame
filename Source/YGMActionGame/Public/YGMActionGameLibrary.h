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
    static float GetDistance(const FVector& Location1, const FVector& Location2);
    static EDir GetDirection(const AActor* MainActor, const FVector& TargetLocation);
    static float GetMovementDirection(const FVector& Velocity, const FVector& Forward);
    static bool CheckPercentage(const int32 Percent);
};
