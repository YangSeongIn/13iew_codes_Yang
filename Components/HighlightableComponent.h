/*
* Header "HighlightableComponent"
* Created by Yang
* Last Modified by Yang
* Description:
* Create a highlight when focused on an interactive object in first person mode.
*/

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HighlightableComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CAP2_API UHighlightableComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:	
	bool bIsOnFocus;
	int8 DoN = 0;

	UPROPERTY()
		class AMainCharacter* MainCharacter;
	UPROPERTY()
		UStaticMeshComponent* StaticMesh;
	UPROPERTY()
		class UCableComponent* Cable;

	UFUNCTION()
		void TurnOnHighlight();
	//
	// A function connected a LatentActionInfo Structure for repeating the operation of highlighting 
	// when focusing and erasing the highlighting after a certain period of time.
	//
	UFUNCTION()
		void DelayFunc();
};
