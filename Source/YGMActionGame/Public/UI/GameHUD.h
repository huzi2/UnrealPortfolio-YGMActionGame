// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

class UPlayerOverlay;
/**
 * 
 */
UCLASS()
class YGMACTIONGAME_API AGameHUD : public AHUD
{
	GENERATED_BODY()

private:
	virtual void BeginPlay() override;

public:
	FORCEINLINE UPlayerOverlay* GetPlayerOverlay() const { return PlayerOverlay; }

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerOverlay> PlayerOverlayClass;

	UPROPERTY()
	UPlayerOverlay* PlayerOverlay;
};
