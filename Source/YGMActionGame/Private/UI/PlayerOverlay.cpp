// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerOverlay.h"
#include "Components/ProgressBar.h"

void UPlayerOverlay::SetHealthBarPercent(const float Percent)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(Percent);
	}
}

void UPlayerOverlay::SetStaminaBarPercent(const float Percent)
{
	if (StaminaProgressBar)
	{
		StaminaProgressBar->SetPercent(Percent);
	}
}
