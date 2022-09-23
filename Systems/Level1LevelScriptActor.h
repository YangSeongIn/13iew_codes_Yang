/*
* Header "Level2LevelScriptActor"
* Created by Yang
* Last Modified by Yang
* Description:
* Level1 level script
*/

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "CheckPoint.h"
#include "Level1Structs.h"
#include "Engine/TriggerBox.h"

#include "Level1LevelScriptActor.generated.h"

UCLASS()
class CAP2_API ALevel1LevelScriptActor : public ALevelScriptActor
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
		bool bDoOnceGenerateCorpse = true;
	UPROPERTY(BlueprintReadWrite)
		bool bDoOncePlayDoctorAppearanceSequence = true;
	UPROPERTY(BlueprintReadWrite)
		bool bDoOncePlayCorpseMoving = true;

protected:
	virtual void BeginPlay() override;
};
