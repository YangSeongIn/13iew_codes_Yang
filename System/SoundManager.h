// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SoundManager.generated.h"

UCLASS()
class CAP2_API ASoundManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ASoundManager();

	void MakeSoundTurnOnFlashLight(FVector FlashLightLocation);
	void PlayLeverSound(FVector FlashLightLocation);
	UFUNCTION(BlueprintCallable)
		void PlayConveyorBeltSound(FVector ConveyorBeltLocation);
	UFUNCTION(BlueprintCallable)
		void StopConveyBeltSound();
	UFUNCTION(BlueprintCallable)
		void PlayPullBoardSound(FVector PullBoardLocation);
	UFUNCTION(BlueprintCallable)
		void StopPullBoardSound();
	UFUNCTION(BlueprintCallable)
		void PlayWoodBrokenSound(FVector WoodLocation);
	UFUNCTION(BlueprintCallable)
		void PlayTapedMouthSound(FVector TapedMouthLocation);
	UFUNCTION(BlueprintCallable)
		void PlayDrawerOpenSound(FVector DrawerLocation);
	UFUNCTION(BlueprintCallable)
		void PlayDrawerCloseSound(FVector DrawerLocation);
	UFUNCTION(BlueprintCallable)
		void PlayDoorToggleSound(FVector DoorLocation);
	UFUNCTION(BlueprintCallable)
		UAudioComponent* PlayValveGaugeHorrorSound();
	UFUNCTION(BlueprintCallable)
		void PlayEnemyFootStepSound(FVector Loc);
	UFUNCTION(BlueprintCallable)
		void StopEnemyFootStepSound();

	UPROPERTY(EditAnywhere)
		class USoundCue* LeverSoundCue;
	UPROPERTY(EditAnywhere)
		class USoundCue* ConveyorSoundCue;
	UPROPERTY(EditAnywhere)
		class USoundCue* TurnOnFlashLightSoundCue;
	UPROPERTY(EditAnywhere)
		class USoundCue* PullBoardSoundCue;
	UPROPERTY(EditAnywhere)
		class USoundCue* WoodBrokenSoundCue;
	UPROPERTY(EditAnywhere)
		class USoundCue* TapedMouthSoundCue;
	UPROPERTY(EditAnywhere)
		class USoundCue* DrawerOpenSoundCue;
	UPROPERTY(EditAnywhere)
		class USoundCue* DrawerCloseSoundCue;
	UPROPERTY(EditAnywhere)
		class USoundCue* DoorToggleSoundCue;
	UPROPERTY(EditAnywhere)
		class USoundCue* ValveGaugeHorrorSoundCue;
	UPROPERTY(EditAnywhere)
		class USoundCue* EnemyFootStepSoundCue;

	UPROPERTY(BlueprintReadWrite)
		class UAudioComponent* ConveyorBeltSound;
	UPROPERTY(BlueprintReadWrite)
		class UAudioComponent* PullBoardSound;
	UPROPERTY(BlueprintReadWrite)
		class UAudioComponent* ValveGaugeHorrorSound;
	UPROPERTY(BlueprintReadWrite)
		class UAudioComponent* EnemyFootStepSound;

protected:
	virtual void BeginPlay() override;
};
