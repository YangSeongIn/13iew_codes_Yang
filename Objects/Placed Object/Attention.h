/*
* Header "Attention"
* Created by Yang
* Last Modified by Yang
* Description:
* The character's head turns towards this object for character's, game's liveness.
* Rotate character's head in MainCharacterAnimInstance.cpp.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Attention.generated.h"

UCLASS()
class CAP2_API AAttention : public AActor
{
	GENERATED_BODY()
	
public:	
	AAttention();
	UPROPERTY()
		class AMainCharacter* MainCharacter;

	UPROPERTY(EditAnywhere)
		float SphereSize;

public:	
	virtual void Tick(float DeltaTime) override;
};
