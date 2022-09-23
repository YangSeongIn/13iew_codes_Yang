/*
* Header "AnimNotifyState_Ladder"
* Created by Yang
* Last Modified by Yang
* Description: 
* Makes a sound every time character's hand touches the ladder.
* Makes a different sound depending on the material during ladder mode.
*/

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Sound/SoundCue.h"

#include "AnimNotifyState_Ladder.generated.h"

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
