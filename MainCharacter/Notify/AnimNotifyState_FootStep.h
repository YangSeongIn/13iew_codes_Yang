// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Sound/SoundCue.h"

#include "AnimNotifyState_FootStep.generated.h"

/**
 * 
 */
UCLASS()
class CAP2_API UAnimNotifyState_FootStep : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UAnimNotifyState_FootStep();

	class AMainCharacter* MainCharacter;

	USoundCue* ConcreteSoundCue;
	USoundCue* CarpetSoundCue;
	USoundCue* WoodSoundCue;
	USoundCue* GrassSoundCue;
	USoundCue* GravelSoundCue;
	USoundCue* GroundSoundCue;
	USoundCue* MetalSoundCue;
	USoundCue* PlasticSoundCue;
	USoundCue* SandSoundCue;
	USoundCue* SnowSoundCue;
	USoundCue* WaterSoundCue;
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
};
