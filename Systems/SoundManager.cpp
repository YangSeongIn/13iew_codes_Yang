#include "SoundManager.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

ASoundManager::ASoundManager()
{
	PrimaryActorTick.bCanEverTick = false;

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

void ASoundManager::PlayButtonSound(FVector ButtonLocation)
{
	if (ButtonSoundCue)
	{
		ButtonSound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ButtonSoundCue, ButtonLocation);
	}
}

void ASoundManager::StopButtonSound()
{
	if (ButtonSound && ButtonSound->IsPlaying())
	{
		ButtonSound->Stop();
		ButtonSound = nullptr;
	}
}