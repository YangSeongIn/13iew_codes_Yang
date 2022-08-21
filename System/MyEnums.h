// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "MyEnums.generated.h"

UENUM(BlueprintType)
enum class ECheckPointRespawnType : uint8
{
	None UMETA(DisplayName = "None"),
	Crawling UMETA(DisplayName = "Crawling"),
	Getup UMETA(DisplayName = "Getup"),
	WalkIn UMETA(DisplayName = "WalkIn")
};
