#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Cap2GameModeBase.generated.h"

class AMainCharacter;

UCLASS()
class CAP2_API ACap2GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMainCharacter* MainCharacter;
	class UMyGameInstance* GameInstance;

	UPROPERTY()
		float RespawnTime;

	UFUNCTION()
		void EventOnMainCharacterDie();

protected:
	virtual void BeginPlay() override;
};