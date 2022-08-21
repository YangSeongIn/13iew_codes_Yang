// Fill out your copyright notice in the Description page of Project Settings.

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

	//FString RespawnActionEnumToString;
	ECheckPointRespawnType CheckPointRespawnType = ECheckPointRespawnType::Crawling;

	class AMainCharacter* MainCharacter;
	class USlotDetailWidget* SlotDetailWidget;

	UPROPERTY()
		class USaveGameObject* SaveGameData;

	UFUNCTION()
		void LoadGame();
	UFUNCTION()
		void SaveData();
	UFUNCTION(BlueprintCallable)
		void LoadData();
	UFUNCTION()
		void DeleteData();

	FOnReset OnReset;
	FOnSave OnSave;

	/* Level Array */
	TArray<FString> LevelArray =
	{
		"Level1",
		"Level2",
	};
};
