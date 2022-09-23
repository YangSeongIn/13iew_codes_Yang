/*
* Header "MyEnums"
* Created by Yang
* Last Modified by Yang
* Description:
* Enum for having different animations of the playing character depending on where it is loaded when respawned.
*/

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
