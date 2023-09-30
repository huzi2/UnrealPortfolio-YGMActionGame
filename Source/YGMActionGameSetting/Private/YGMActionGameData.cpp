// Fill out your copyright notice in the Description page of Project Settings.


#include "YGMActionGameData.h"

UYGMActionGameData::UYGMActionGameData()
{
    const FString PlayerAttackActionInfoJsonFilePath = FPaths::ProjectDir() + TEXT("/Data/PlayerAttackActionInfo.json");
    ReadActionInfoJsonData(PlayerAttackActionInfoJsonFilePath, PlayerAttackActionInfo);

    const FString PlayerSmashActionInfoJsonFilePath = FPaths::ProjectDir() + TEXT("/Data/PlayerSmashActionInfo.json");
    ReadActionInfoJsonData(PlayerSmashActionInfoJsonFilePath, PlayerSmashActionInfo);
}

void UYGMActionGameData::ReadActionInfoJsonData(const FString& FilePath, TMap<ECharacterState, std::pair<FName, ECharacterState>>& InfoMap)
{
    FString JsonFileContent;

    // 파일의 내용을 JsonFileContent에 로드
    if (FFileHelper::LoadFileToString(JsonFileContent, *FilePath))
    {
        TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonFileContent);
        TSharedPtr<FJsonObject> JsonObject;

        // JSON 텍스트를 FJsonObject로 파싱
        if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
        {
            UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ECharacterState"), true);

            // FJsonObject의 멤버들을 반복
            for (auto& Member : JsonObject->Values)
            {
                TSharedPtr<FJsonObject> Value = Member.Value->AsObject();

                // 키와 값을 출력
                if (Value.IsValid() && Value->HasField(TEXT("animation")) && Value->HasField(TEXT("nextState")))
                {
                    ECharacterState Key = static_cast<ECharacterState>(EnumPtr->GetValueByNameString(Member.Key));
                    FName Animation = FName(*Value->GetStringField(TEXT("animation")));
                    ECharacterState NextState = static_cast<ECharacterState>(EnumPtr->GetValueByNameString(Value->GetStringField(TEXT("nextState"))));

                    InfoMap.Add(Key, std::make_pair(Animation, NextState));
                }
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to parse JSON"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load JSON file"));
    }
}
