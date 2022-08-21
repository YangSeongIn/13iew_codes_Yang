// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_PullBoard.generated.h"

/**
 * 
 */
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
