// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "XMLFileReader.generated.h"

class FXmlNode;

UCLASS()
class YGMACTIONGAMESETTING_API UXMLFileReader : public UObject
{
    GENERATED_BODY()

public:
    static const FXmlNode* ParsingXML(const FString& FilePath);
};
