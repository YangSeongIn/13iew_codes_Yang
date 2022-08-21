// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundManager.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

ASoundManager::ASoundManager()
{
	PrimaryActorTick.bCanEverTick = false;

	/*static ConstructorHelpers::FObjectFinder<USoundCue> FlashLightSoundCueAsset(TEXT("SoundCue'/Game/Sounds/FlashLight/Cue_TurnOnFlashLight.Cue_TurnOnFlashLight'"));
	if (FlashLightSoundCueAsset.Succeeded())
	{
		TurnOnFlashLightSoundCue = FlashLightSoundCueAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> LeverSoundCueAsset(TEXT("SoundCue'/Game/Sounds/Lever/Cue_Lever.Cue_Lever'"));
	if (LeverSoundCueAsset.Succeeded())
	{
		LeverSoundCue = LeverSoundCueAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> ConveyorSoundCueAsset(TEXT("SoundCue'/Game/Sounds/ConveyorBelt/Cue_ConveyorBelt.Cue_ConveyorBelt'"));
	if (ConveyorSoundCueAsset.Succeeded())
	{
		ConveyorSoundCue = ConveyorSoundCueAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> PullBoardSoundCueAsset(TEXT("SoundCue'/Game/Sounds/PullBoard/PullBoard_Cue.PullBoard_Cue'"));
	if (PullBoardSoundCueAsset.Succeeded())
	{
		PullBoardSoundCue = PullBoardSoundCueAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> WoodBrokenSoundCueAsset(TEXT("SoundCue'/Game/Sounds/PullBoard/WoodBroken_Cue.WoodBroken_Cue'"));
	if (WoodBrokenSoundCueAsset.Succeeded())
	{
		WoodBrokenSoundCue = WoodBrokenSoundCueAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> TapedMouthSoundCueAsset(TEXT("SoundCue'/Game/Asset/Horror_Props/Animations/Dead_man/Scream_Cue.Scream_Cue'"));
	if (TapedMouthSoundCueAsset.Succeeded())
	{
		TapedMouthSoundCue = TapedMouthSoundCueAsset.Object;
	}*/
}

void ASoundManager::BeginPlay()
{
	Super::BeginPlay();

}

void ASoundManager::MakeSoundTurnOnFlashLight(FVector FlashLightLocation)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), TurnOnFlashLightSoundCue, FlashLightLocation);
}

void ASoundManager::PlayLeverSound(FVector LeverLocation)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), LeverSoundCue, LeverLocation);
}

void ASoundManager::PlayConveyorBeltSound(FVector ConveyorBeltLocation)
{
	if (ConveyorSoundCue)
	{
		ConveyorBeltSound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ConveyorSoundCue, ConveyorBeltLocation);
	}
}

void ASoundManager::StopConveyBeltSound()
{
	if (ConveyorBeltSound && ConveyorBeltSound->IsPlaying())
	{
		ConveyorBeltSound->Stop();
		ConveyorBeltSound = nullptr;
	}
}

void ASoundManager::PlayPullBoardSound(FVector PullBoardLocation)
{
	if (PullBoardSoundCue)
	{
		PullBoardSound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), PullBoardSoundCue, PullBoardLocation);
	}
}

void ASoundManager::StopPullBoardSound()
{
	if (PullBoardSound && PullBoardSound->IsPlaying())
	{
		PullBoardSound->Stop();
		PullBoardSound = nullptr;
	}
}

void ASoundManager::PlayWoodBrokenSound(FVector WoodLocation)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), WoodBrokenSoundCue, WoodLocation);
}

void ASoundManager::PlayTapedMouthSound(FVector TapedMouthLocation)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), TapedMouthSoundCue, TapedMouthLocation);
}

void ASoundManager::PlayDrawerOpenSound(FVector DrawerLocation)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DrawerOpenSoundCue, DrawerLocation);
}

void ASoundManager::PlayDrawerCloseSound(FVector DrawerLocation)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DrawerCloseSoundCue, DrawerLocation);
}

void ASoundManager::PlayDoorToggleSound(FVector DoorLocation)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DoorToggleSoundCue, DoorLocation);
}

UAudioComponent* ASoundManager::PlayValveGaugeHorrorSound()
{
	ValveGaugeHorrorSound = UGameplayStatics::SpawnSound2D(GetWorld(), ValveGaugeHorrorSoundCue);
	return ValveGaugeHorrorSound;
}

void ASoundManager::PlayEnemyFootStepSound(FVector Loc)
{
	if(EnemyFootStepSoundCue)
		EnemyFootStepSound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), EnemyFootStepSoundCue, Loc);
}

void ASoundManager::StopEnemyFootStepSound()
{
	if (EnemyFootStepSound && EnemyFootStepSound->IsPlaying())
	{
		EnemyFootStepSound->Stop();
		EnemyFootStepSound = nullptr;
	}
}