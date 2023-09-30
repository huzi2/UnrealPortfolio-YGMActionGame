// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameHUD.h"
#include "UI/PlayerOverlay.h"

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerOverlayClass)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			APlayerController* Controller = World->GetFirstPlayerController();
			if (Controller)
			{
				PlayerOverlay = CreateWidget<UPlayerOverlay>(Controller, PlayerOverlayClass);
				if (PlayerOverlay)
				{
					PlayerOverlay->AddToViewport();
				}
			}
		}
	}
}
