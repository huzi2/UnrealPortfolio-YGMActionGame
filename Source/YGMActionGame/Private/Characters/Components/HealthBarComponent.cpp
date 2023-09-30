// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Components/HealthBarComponent.h"
#include "UI/HealthBar.h"
#include "Components/ProgressBar.h"

void UHealthBarComponent::SetHealthBarPercent(const float Percent)
{
	if (!HealthBarWidget)
	{
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}

	if (HealthBarWidget && HealthBarWidget->HealthBar)
	{
		HealthBarWidget->HealthBar->SetPercent(Percent);
	}
}
