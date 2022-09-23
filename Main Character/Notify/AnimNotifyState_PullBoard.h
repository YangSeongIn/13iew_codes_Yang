/*
* Header "AnimNotifyState_PullBoard"
* Created by Yang
* Last Modified by Yang
* Description: 
* Makes a sound board crack sound when character pulls board.
* Makes a different sound depending on the material during pull-board mode.
*/

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_PullBoard.generated.h"

UCLASS()
class CAP2_API UAnimNotifyState_PullBoard : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UAnimNotifyState_PullBoard();

	class AMainCharacter* MainCharacter;

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
};
