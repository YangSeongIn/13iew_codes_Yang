/*
* Header "SaveGameObject"
* Created by Yang
* Last Modified by Yang
* Description:
* A class that saves all information about the game in that slot.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Level1Structs.h"
#include "Level2Structs.h"
#include "MainCharacterStructs.h"
#include "MyEnums.h"

#include "SaveGameObject.generated.h"

UCLASS()
class CAP2_API USaveGameObject : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
		float TotalPlayTime;
	UPROPERTY()
		FString SaveLevelName = "Level2_SaveTest";
	UPROPERTY()
		FString SavePointName = "None";
	UPROPERTY()
		ECheckPointRespawnType SavePointType = ECheckPointRespawnType::Crawling;

	UPROPERTY(EditAnywhere)
		FMainCharacterStruct MainCharacterStruct;
	UPROPERTY()
		bool bHaveFlashLight = false;

	UPROPERTY()
		FLevel1Struct Level1SaveDataStruct;
	UPROPERTY()
		FLevel2Struct Level2SaveDataStruct;
};