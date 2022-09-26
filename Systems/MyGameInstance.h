/*
* Header "MyGameInstance"
* Created by Yang
* Last Modified by Yang
* Description:
* Saving the game in each slot or loading the game information according to the information contained therein using the delegate.
* Manages total game play time, saved location information, and names of all levels
*/

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyEnums.h"

#include "MyGameInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnSave);
DECLARE_MULTICAST_DELEGATE(FOnReset);

UCLASS()
class CAP2_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// For Saving Total Play Time
	float EndTime;
	FString SaveSlotName;
	FString SaveLevelName;

	ECheckPointRespawnType CheckPointRespawnType = ECheckPointRespawnType::Crawling;

	class AMainCharacter* MainCharacter;
	class USlotDetailWidget* SlotDetailWidget;

	UPROPERTY()
		class USaveGameObject* SaveGameData;

	UFUNCTION()
		void LoadGame();
	UFUNCTION()
		void SaveData(FVector LocToSave);
	UFUNCTION(BlueprintCallable)
		void LoadData();
	UFUNCTION()
		void DeleteData();
	UFUNCTION()
		void SaveTotalTime();

	FOnReset OnReset;
	FOnSave OnSave;

	/* Level Array */
	// if you want to fix it, you only have to fix 'SaveGameObject.h->SaveLevelName'.
	TArray<FString> LevelArray =
	{
		"Level1",
		//"Level2_SaveTest",
		"Level2"
	};
};
