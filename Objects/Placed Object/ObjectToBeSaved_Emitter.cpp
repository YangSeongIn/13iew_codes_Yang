#include "ObjectToBeSaved_Emitter.h"
#include "Particles/ParticleSystem.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "MyGameInstance.h"
#include "SaveGameObject.h"
#include "Kismet/GameplayStatics.h"

AObjectToBeSaved_Emitter::AObjectToBeSaved_Emitter()
{
	Emitter_1 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Emitter_1"));
	Emitter_1->SetupAttachment(RootComponent);
	Emitter_2 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Emitter_2"));
	Emitter_2->SetupAttachment(RootComponent);
}

void AObjectToBeSaved_Emitter::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->OnReset.AddUFunction(this, FName("Reset"));
		GameInstance->OnSave.AddUFunction(this, FName("Save"));
	}
}

void AObjectToBeSaved_Emitter::Save()
{
	Super::Save();
	if (bSaveVisibility_Emitter)
	{
		GameInstance->SaveGameData->Level1SaveDataStruct.ObjectToBeSavedData_Visibility.Add(this, Emitter_1->IsVisible());
	}
}

void AObjectToBeSaved_Emitter::Reset()
{
	Super::Reset();
	if (GameInstance->SaveGameData->Level1SaveDataStruct.ObjectToBeSavedData_Visibility.Contains(this))
	{
		bool b = *GameInstance->SaveGameData->Level1SaveDataStruct.ObjectToBeSavedData_Visibility.Find(this);
		if (bSaveVisibility_Emitter)
		{
			Emitter_1->SetVisibility(b);
			Emitter_2->SetVisibility(b);
		}
	}
}
