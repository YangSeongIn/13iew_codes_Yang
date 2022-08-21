// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SaveGameObject.h"
#include "MainCharacter.h"
#include "Master_Movable.h"
#include "InteractableObject_Door.h"
#include "CheckPoint.h"

void UMyGameInstance::LoadGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		SaveGameData = Cast<USaveGameObject>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	}
	else
	{
		SaveGameData = Cast<USaveGameObject>(UGameplayStatics::CreateSaveGameObject(USaveGameObject::StaticClass()));
	}

}

void UMyGameInstance::SaveData()
{
	if (SaveGameData)
	{
		if (OnSave.IsBound() == true)
		{
			OnSave.Broadcast();

			EndTime = UGameplayStatics::GetTimeSeconds(GetWorld());
			SaveGameData->TotalPlayTime += EndTime;
			SaveLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
			SaveGameData->SaveLevelName = SaveLevelName;
			SaveGameData->SavePointType = CheckPointRespawnType;

			//if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == LevelArray[0])
			if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == TEXT("Main_Yang"))
			{
				SaveGameData->Level1SaveDataStruct.MainCharacterLocation = MainCharacter->GetActorLocation();
			}
			else if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == LevelArray[1])
			{
				SaveGameData->Level2SaveDataStruct.MainCharacterLocation = MainCharacter->GetActorLocation();
			}

			UGameplayStatics::SaveGameToSlot(SaveGameData, SaveSlotName, 0);
		}

	}
}

void UMyGameInstance::LoadData()
{
	// Main Character Data Load
	if (SaveGameData)
	{
		if (OnReset.IsBound() == true)
		{
			OnReset.Broadcast();
			MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			CheckPointRespawnType = SaveGameData->SavePointType;

			if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == LevelArray[0])
			//if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == TEXT("Main_Yang"))
			{
				MainCharacter->SetActorLocation(SaveGameData->Level1SaveDataStruct.MainCharacterLocation);
			}
			/*else if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == LevelArray[1])
			{
				MainCharacter->SetActorLocation(SaveGameData->Level2SaveDataStruct.MainCharacterLocation);
			}*/
		}
	}
}

void UMyGameInstance::DeleteData()
{
	UGameplayStatics::DeleteGameInSlot(SaveSlotName, 0);
}