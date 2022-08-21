// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneratorUsingTimer.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Master_Corpse.h"
#include "SaveGameObject.h"
#include "MyGameInstance.h"

// Sets default values
AGeneratorUsingTimer::AGeneratorUsingTimer()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGeneratorUsingTimer::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->OnReset.AddUFunction(this, FName("Reset"));
		GameInstance->OnSave.AddUFunction(this, FName("Save"));
	}

	Delegate.BindUFunction(this, "GenerateCorpse", this);
}

void AGeneratorUsingTimer::ActivateCorpseGenerator()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, SpawnTimeDelay, true);
}

void AGeneratorUsingTimer::GenerateCorpse(AMaster_Corpse* C)
{
	bIsOnCorpseGenerator = true;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	FRotator Rotator = FRotator(0, 0, 0);
	FVector  SpawnLocation = GetActorLocation();
	UWorld* World = GetWorld();

	World->SpawnActor<AActor>(AMaster_Corpse::StaticClass(), SpawnLocation, Rotator, SpawnParams);
}

void AGeneratorUsingTimer::DeActivateCorpseGenerator()
{
	bIsOnCorpseGenerator = false;

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_AttackDelay);
}

void AGeneratorUsingTimer::Reset()
{
	if (GameInstance->SaveGameData->Level1SaveDataStruct.CorpseGeneratorData.Contains(this))
	{
		bIsOnCorpseGenerator = *GameInstance->SaveGameData->Level1SaveDataStruct.CorpseGeneratorData.Find(this);
	}

	if (bIsOnCorpseGenerator)
	{
		ActivateCorpseGenerator();
	}
	else
	{
		DeActivateCorpseGenerator();
	}
}
void AGeneratorUsingTimer::Save()
{
	GameInstance->SaveGameData->Level1SaveDataStruct.CorpseGeneratorData.Add(this, bIsOnCorpseGenerator);
}