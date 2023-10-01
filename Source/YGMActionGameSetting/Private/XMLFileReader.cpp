// Copyright Epic Games, Inc. All Rights Reserved.

#include "XMLFileReader.h"
#include "XmlParser.h"

const FXmlNode* UXMLFileReader::ParsingXML(const FString& FilePath)
{
    // XML 파일 읽기
    FString XmlText;
    FXmlNode* RootNode = nullptr;

    if (FFileHelper::LoadFileToString(XmlText, *FilePath))
    {
        // XML 파일 파싱
        FXmlFile XmlFile(XmlText); // TODO : 여기서 제대로 파일이 안읽히고 있음. 확인 필요
        if (XmlFile.IsValid())
        {
            RootNode = XmlFile.GetRootNode();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to parse XML"));
            UE_LOG(LogTemp, Warning, TEXT("XML is not valid: %s"), *XmlFile.GetLastError());
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load XML file"));
    }
    return RootNode;
}
