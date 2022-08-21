// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckPoint.h"
#include "MainCharacter.h"
#include "Cap2GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"
#include "Master_Movable.h"
#include "SaveGameObject.h"
#include "FirstPersonDefaultWidget.h"

// Sets default values
ACheckPoint::ACheckPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);

	Box->OnComponentBeginOverlap.AddDynamic(this, &ACheckPoint::OnOverlapBegin);
	Box->OnComponentEndOverlap.AddDynamic(this, &ACheckPoint::OnOverlapEnd);
}

void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->OnReset.AddUFunction(this, FName("Reset"));
		GameInstance->OnSave.AddUFunction(this, FName("Save"));
	}
}

void ACheckPoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	MainCharacter = Cast<AMainCharacter>(OtherActor);
	if (!bIsNextLevel)
	{
		NormalCheckPoint();
	}
	else
	{
		ToNextLevel();
	}
}

void ACheckPoint::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ACheckPoint::NormalCheckPoint()
{
	if (!bIsDestroyed)
	{
		if (MainCharacter)
		{
			MainCharacter->bIsStartingPoint = false;
			GameInstance->CheckPointRespawnType = CheckPointRespawnType;
			SaveGameToSlot();
		}
	}
}

void ACheckPoint::ToNextLevel()
{
	if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == GameInstance->LevelArray[0])
	{
		MainCharacter->bIsStartingPoint = false;
		MainCharacter->DefaultWidget->PlayWidgetAnimation(MainCharacter->DefaultWidget->FadeOut, true);

		GameInstance->SaveGameData->Level1SaveDataStruct.CheckPointData.Empty();
		GameInstance->SaveGameData->Level1SaveDataStruct.MovableData.Empty();
		GameInstance->SaveGameData->Level1SaveDataStruct.DoorData.Empty();
		GameInstance->SaveGameData->Level1SaveDataStruct.LeverData.Empty();
		GameInstance->SaveGameData->Level1SaveDataStruct.SlicableData.Empty();
		GameInstance->SaveGameData->Level1SaveDataStruct.ConveyorBeltData.Empty();
		GameInstance->SaveGameData->Level1SaveDataStruct.CorpseGeneratorData.Empty();
		GameInstance->SaveGameData->Level1SaveDataStruct.ObjectToBeSavedData_Transform.Empty();
		GameInstance->SaveGameData->Level1SaveDataStruct.ObjectToBeSavedData_Visibility.Empty();
		//GameInstance->SaveGameData->Level1SaveDataStruct.DrawerData.Empty();
		GameInstance->SaveGameData->Level1SaveDataStruct.ThrowableData.Empty();
	}
}

void ACheckPoint::SaveGameToSlot()
{
	MainCharacter->MainCharacterLocation = GetActorLocation();
	bIsDestroyed = true;
	GameInstance->SaveData();
}

void ACheckPoint::Reset()
{
	if (GameInstance->SaveGameData->Level1SaveDataStruct.CheckPointData.Contains(this))
	{
		bIsDestroyed = *GameInstance->SaveGameData->Level1SaveDataStruct.CheckPointData.Find(this);
	}
}
void ACheckPoint::Save()
{
	GameInstance->SaveGameData->Level1SaveDataStruct.CheckPointData.Add(this, bIsDestroyed);
	GameInstance->SaveGameData->SavePointName = SavePointName;
}