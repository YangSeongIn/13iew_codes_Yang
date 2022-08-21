// Fill out your copyright notice in the Description page of Project Settings.


#include "Master_Movable.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGameObject.h"
#include "MainCharacter.h"
#include "Components/BoxComponent.h"
#include "HighlightableComponent.h"

// Sets default values
AMaster_Movable::AMaster_Movable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(StaticMesh);
	Box->SetRelativeLocation(FVector(0, 0, 50));

	HightlightableComponent = CreateDefaultSubobject<UHighlightableComponent>(TEXT("HightlightableComponent"));

	DefaultBoxLocation = Box->GetRelativeLocation();
}

// Called when the game starts or when spawned
void AMaster_Movable::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->OnReset.AddUFunction(this, FName("Reset"));
		GameInstance->OnSave.AddUFunction(this, FName("Save"));
	}
}

void AMaster_Movable::Reset()
{
	if (GameInstance->SaveGameData->Level1SaveDataStruct.MovableData.Contains(this))
	{
		SetActorTransform(*GameInstance->SaveGameData->Level1SaveDataStruct.MovableData.Find(this));
	}
}

void AMaster_Movable::Save()
{
	GameInstance->SaveGameData->Level1SaveDataStruct.MovableData.Add(this, GetActorTransform());
}