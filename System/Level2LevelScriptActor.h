// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Level2LevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class CAP2_API ALevel2LevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
		class UMyGameInstance* GameInstance;
	UPROPERTY()
		class AMainCharacter* MainCharacter;

	UPROPERTY()
		float StartTime;
	UPROPERTY()
		float EndTime;

	UPROPERTY(BlueprintReadWrite)
		bool bDoOnceDragMainCharacterCorpse = true;

protected:
	virtual void BeginPlay() override;
};
