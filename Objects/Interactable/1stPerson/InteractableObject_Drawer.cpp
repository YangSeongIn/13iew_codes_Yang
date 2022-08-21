// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObject_Drawer.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "MainCharacter.h"
#include "SoundManager.h"
#include "MyGameInstance.h"
#include "SaveGameObject.h"

AInteractableObject_Drawer::AInteractableObject_Drawer()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);

	DrawerTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DrawerTimeline"));

	InterpFunction.BindUFunction(this, FName("InterpReturn"));
	TimelineFinish.BindUFunction(this, FName("OnFinish"));

	bIsClosed = true;
}

void AInteractableObject_Drawer::BeginPlay()
{
	Super::BeginPlay();

	// Save & Load
	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->OnReset.AddUFunction(this, FName("Reset"));
		GameInstance->OnSave.AddUFunction(this, FName("Save"));
	}

	if (DrawerCurveFloat && DrawerTimeline)
	{
		DrawerTimeline->AddInterpFloat(DrawerCurveFloat, InterpFunction, FName("Drawer"));
		DrawerTimeline->SetTimelineFinishedFunc(TimelineFinish);
		DrawerTimeline->SetLooping(false);
	}

	SoundManager = Cast<ASoundManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ASoundManager::StaticClass()));
}

void AInteractableObject_Drawer::Interact()
{
	InteractCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (SoundManager && InteractCharacter && DrawerTimeline)
	{
		if (bIsClosed)
		{
			SoundManager->PlayDrawerOpenSound(GetActorLocation());
			DrawerTimeline->Play();
		}
		else
		{
			SoundManager->PlayDrawerCloseSound(GetActorLocation());
			DrawerTimeline->Reverse();
		}
		bIsClosed = !bIsClosed;
	}
}

void AInteractableObject_Drawer::OnFinish()
{

}

void AInteractableObject_Drawer::InterpReturn(float value)
{
	StaticMesh->SetRelativeLocation(FVector(0, UKismetMathLibrary::Ease(0, 30, value, EEasingFunc::SinusoidalInOut), 0));
}

void AInteractableObject_Drawer::Reset()
{
	/*if (GameInstance->SaveGameData->Level1SaveDataStruct.DrawerData.Contains(this))
	{
		bIsClosed = *GameInstance->SaveGameData->Level1SaveDataStruct.DrawerData.Find(this);
	}

	if (!bIsClosed)
	{
		SetActorRelativeLocation(FVector(0, 30, 0));
	}*/
}

void AInteractableObject_Drawer::Save()
{
	//GameInstance->SaveGameData->Level1SaveDataStruct.DrawerData.Add(this, bIsClosed);
}