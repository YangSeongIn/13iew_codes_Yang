/*
* Header "Level2LevelScriptActor"
* Created by Yang
* Last Modified by Yang
* Description:
* Level2 level script
*/

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Level2LevelScriptActor.generated.h"

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
	UPROPERTY(BlueprintReadWrite)
		bool bDoOnceDisableToiletDoor = true;
	UPROPERTY(BlueprintReadWrite)
		bool bDoOnceVentKushi = true;
	UPROPERTY(BlueprintReadWrite)
		bool bDoOnceToiletEvent = true;

protected:
	virtual void BeginPlay() override;
};
