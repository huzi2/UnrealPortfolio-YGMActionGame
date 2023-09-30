// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerOverlay.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class YGMACTIONGAME_API UPlayerOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetHealthBarPercent(const float Percent);
	void SetStaminaBarPercent(const float Percent);

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgressBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaProgressBar;
};
