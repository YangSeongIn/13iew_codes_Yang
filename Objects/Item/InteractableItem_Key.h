/*
* Header "InteractableItem_Key.h"
* Created by Yang
* Last Modified by Yang
* Description:
* To open the door.
*/

#pragma once

#include "CoreMinimal.h"
#include "Master_InteractableItem.h"
#include "InteractableItem_Key.generated.h"

UCLASS()
class CAP2_API AInteractableItem_Key : public AMaster_InteractableItem
{
	GENERATED_BODY()

public:
	AInteractableItem_Key();
	
	class AMainCharacter* MainCharacter;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> KeyInventoryWidgetClass;
	UPROPERTY()
		class UKeyInventoryWidget* KeyInventoryWidget;

	void GetKey();
	void OnKeyInventoryWidget();
	void OffKeyInventoryWidget();

public:
	virtual void BeginPlay() override;
	virtual void Interact() override;
};
