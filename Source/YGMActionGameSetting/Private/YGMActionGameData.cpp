// Fill out your copyright notice in the Description page of Project Settings.

#include "YGMActionGameData.h"
#include "JsonFileReader.h"
#include "XMLFileReader.h"
#include "XmlParser.h"

UYGMActionGameData::UYGMActionGameData()
{
    const FString PlayerAttackActionInfoJsonFilePath = FPaths::ProjectContentDir() + TEXT("/Data/PlayerAttackActionInfo.json");
    ReadActionInfoJsonData(PlayerAttackActionInfoJsonFilePath, PlayerAttackActionInfo);

    const FString PlayerSmashActionInfoJsonFilePath = FPaths::ProjectContentDir() + TEXT("/Data/PlayerSmashActionInfo.json");
    ReadActionInfoJsonData(PlayerSmashActionInfoJsonFilePath, PlayerSmashActionInfo);

    //const FString PlayerSmashActionInfoXMLFilePath = FPaths::ProjectContentDir() + TEXT("/Data/PlayerSmashActionInfo.xml");
    /*const FString PlayerSmashActionInfoXMLFilePath = FPaths::ProjectContentDir() / TEXT("/Data/PlayerSmashActionInfo.xml");
    ReadActionInfoXMLData(PlayerSmashActionInfoXMLFilePath, PlayerSmashActionInfo);*/
}

void UYGMActionGameData::ReadActionInfoJsonData(const FString& FilePath, TMap<ECharacterState, TPair<FName, ECharacterState>>& InfoMap)
{
    const TSharedPtr<FJsonObject> JsonObject = UJsonFileReader::ParsingJsonObject(FilePath);
    if (JsonObject.IsValid())
    {
        UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ECharacterState"), true);

        // FJsonObject의 멤버들을 반복
        for (const auto& Member : JsonObject->Values)
        {
            const TSharedPtr<FJsonObject> Value = Member.Value->AsObject();

            // 키와 값을 출력
            if (Value.IsValid() && Value->HasField(TEXT("animation")) && Value->HasField(TEXT("nextState")))
            {
                const ECharacterState Key = static_cast<ECharacterState>(EnumPtr->GetValueByNameString(Member.Key));
                const FName Animation = FName(*Value->GetStringField(TEXT("animation")));
                const ECharacterState NextState = static_cast<ECharacterState>(EnumPtr->GetValueByNameString(Value->GetStringField(TEXT("nextState"))));

                InfoMap.Add(Key, TPair<FName, ECharacterState>(Animation, NextState));
            }
        }
    }
}

void UYGMActionGameData::ReadActionInfoXMLData(const FString& FilePath, TMap<ECharacterState, TPair<FName, ECharacterState>>& InfoMap)
{
    const FXmlNode* RootNode = UXMLFileReader::ParsingXML(FilePath);
    if (!RootNode) return;

    UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ECharacterState"), true);

    for (auto StateNode : RootNode->GetChildrenNodes())
    {
        // 상태, 애니메이션, 다음 상태 파싱
        ECharacterState State = static_cast<ECharacterState>(EnumPtr->GetValueByNameString(TEXT("state")));
        FName Animation = *StateNode->GetAttribute(TEXT("animation"));
        ECharacterState NextState = static_cast<ECharacterState>(EnumPtr->GetValueByNameString(TEXT("nextState")));

        InfoMap.Add(State, TPair<FName, ECharacterState>(Animation, NextState));
    }
}
