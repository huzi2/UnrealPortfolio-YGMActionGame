// Copyright Epic Games, Inc. All Rights Reserved.

#include "XMLFileReader.h"
#include "XmlParser.h"

const FXmlNode* UXMLFileReader::ParsingXML(const FString& FilePath)
{
    // XML ���� �б�
    FString XmlText;
    FXmlNode* RootNode = nullptr;

    if (FFileHelper::LoadFileToString(XmlText, *FilePath))
    {
        // XML ���� �Ľ�
        FXmlFile XmlFile(XmlText); // TODO : ���⼭ ����� ������ �������� ����. Ȯ�� �ʿ�
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
