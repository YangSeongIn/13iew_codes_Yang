/*
* Header "AnimNotifyState_FootStep"
* Created by Yang
* Last Modified by Yang
* Description: Make different sound depending on the material during default state. 
*/

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Sound/SoundCue.h"

#include "AnimNotifyState_FootStep.generated.h"

UCLASS()
class CAP2_API UAnimNotifyState_FootStep : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UAnimNotifyState_FootStep();
	UPROPERTY()
		class AMainCharacter* MainCharacter;
	UPROPERTY()
		USoundCue* ConcreteSoundCue;
	UPROPERTY()
		USoundCue* CarpetSoundCue;
	UPROPERTY()
		USoundCue* WoodSoundCue;
	UPROPERTY()
		USoundCue* GrassSoundCue;
	UPROPERTY()
		USoundCue* GravelSoundCue;
	UPROPERTY()
		USoundCue* GroundSoundCue;
	UPROPERTY()
		USoundCue* MetalSoundCue;
	UPROPERTY()
		USoundCue* PlasticSoundCue;
	UPROPERTY()
		USoundCue* SandSoundCue;
	UPROPERTY()
		USoundCue* SnowSoundCue;
	UPROPERTY()
		USoundCue* WaterSoundCue;
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
};
