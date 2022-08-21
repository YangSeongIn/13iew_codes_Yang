// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Sound/SoundCue.h"

#include "AnimNotifyState_Ladder.generated.h"

/**
 * 
 */
UCLASS()
class CAP2_API UAnimNotifyState_Ladder : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UAnimNotifyState_Ladder();

	class AMainCharacter* MainCharacter;

	USoundCue* LadderSoundCue;

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
};
