// Copyright Epic Games, Inc. All Rights Reserved.

#include "JsonFileReader.h"

const TSharedPtr<FJsonObject> UJsonFileReader::ParsingJsonObject(const FString& FilePath)
{
    FString JsonFileContent;
    TSharedPtr<FJsonObject> JsonObject = nullptr;

    // ������ ������ JsonFileContent�� �ε�
    if (FFileHelper::LoadFileToString(JsonFileContent, *FilePath))
    {
        const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonFileContent);

        // JSON �ؽ�Ʈ�� FJsonObject�� �Ľ�
        if (!FJsonSerializer::Deserialize(JsonReader, JsonObject))
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to parse JSON"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load JSON file"));
    }
    return JsonObject;
}
