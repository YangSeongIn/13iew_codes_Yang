#include "ObjectToBeSaved.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"
#include "SaveGameObject.h"

AObjectToBeSaved::AObjectToBeSaved()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;


}

void AObjectToBeSaved::BeginPlay()
{
	Super::BeginPlay();

	InitTransform = GetActorTransform();

	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->OnReset.AddUFunction(this, FName("Reset"));
		GameInstance->OnSave.AddUFunction(this, FName("Save"));
	}
}

void AObjectToBeSaved::Reset()
{
	if (GameInstance->SaveGameData->Level1SaveDataStruct.ObjectToBeSavedData_Transform.Contains(this))
	{
		bool b = *GameInstance->SaveGameData->Level1SaveDataStruct.ObjectToBeSavedData_Transform.Find(this);
		if (bSaveTransform)
		{
			bIsTransform = b;
			if (bIsTransform)
				SetActorTransform(ChangedTransform);
			else
				SetActorTransform(InitTransform);
		}
	}
	if (GameInstance->SaveGameData->Level1SaveDataStruct.ObjectToBeSavedData_Visibility.Contains(this))
	{
		bool b = *GameInstance->SaveGameData->Level1SaveDataStruct.ObjectToBeSavedData_Visibility.Find(this);
		if (bSaveVisibility)
		{
			StaticMesh->SetVisibility(b);
			if (b)
				StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

void AObjectToBeSaved::Save()
{
	if (bSaveTransform)
	{
		GameInstance->SaveGameData->Level1SaveDataStruct.ObjectToBeSavedData_Transform.Add(this, bIsTransform);
	}
	if (bSaveVisibility)
	{
		GameInstance->SaveGameData->Level1SaveDataStruct.ObjectToBeSavedData_Transform.Add(this, StaticMesh->IsVisible());
	}
}