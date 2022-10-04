/*
* Header "InteractableItem_FlashLight.h"
* Created by Yang
* Last Modified by Yang
* Description:
* Character's flash light.
*/

#pragma once

#include "CoreMinimal.h"
#include "Master_InteractableItem.h"
#include "InteractableItem_FlashLight.generated.h"

UCLASS()
class CAP2_API AInteractableItem_FlashLight : public AMaster_InteractableItem
{
	GENERATED_BODY()

public:
	AInteractableItem_FlashLight();

	bool bIsDestroyed = false;

	UPROPERTY()
		class AMainCharacter* MainCharacter;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> _KeyGuideWidget;
	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
		class UKeyGuideWidget* KeyGuideWidget;

	UPROPERTY()
		class UMyGameInstance* GameInstance;
	UFUNCTION()
		void Reset();
	UFUNCTION()
		void Save();

public:
	virtual void BeginPlay() override;
	virtual void Interact() override;
};
