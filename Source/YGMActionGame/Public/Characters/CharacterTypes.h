// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EDir : uint8
{
	ED_Front UMETA(DisplayName = "Front"),
	ED_Left UMETA(DisplayName = "Left"),
	ED_Right UMETA(DisplayName = "Right"),
	ED_Back UMETA(DisplayName = "Back")
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_NoState UMETA(DisplayName = "NoState"),
	EES_Dead UMETA(DisplayName = "Dead"),
	EES_Idle UMETA(DisplayName = "Idle"),
	EES_Check UMETA(DisplayName = "Check"),
	EES_Rotate UMETA(DisplayName = "Rotate"),
	EES_Dash UMETA(DisplayName = "Dash"),
	EES_Attack UMETA(DisplayName = "Attack")
};
