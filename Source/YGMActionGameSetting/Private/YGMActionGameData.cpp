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

        // FJsonObject�� ������� �ݺ�
        for (const auto& Member : JsonObject->Values)
        {
            const TSharedPtr<FJsonObject> Value = Member.Value->AsObject();

            // Ű�� ���� ���
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
        // ����, �ִϸ��̼�, ���� ���� �Ľ�
        ECharacterState State = static_cast<ECharacterState>(EnumPtr->GetValueByNameString(TEXT("state")));
        FName Animation = *StateNode->GetAttribute(TEXT("animation"));
        ECharacterState NextState = static_cast<ECharacterState>(EnumPtr->GetValueByNameString(TEXT("nextState")));

        InfoMap.Add(State, TPair<FName, ECharacterState>(Animation, NextState));
    }
}
