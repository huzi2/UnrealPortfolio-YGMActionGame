// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "JsonFileReader.generated.h"

UCLASS()
class YGMACTIONGAMESETTING_API UJsonFileReader : public UObject
{
    GENERATED_BODY()

public:
    static const TSharedPtr<FJsonObject> ParsingJsonObject(const FString& FilePath);
};
