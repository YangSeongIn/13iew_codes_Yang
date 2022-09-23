/*
* Header "InteractableObject_ElevatorBtn.h"
* Created by Yang
* Last Modified by Yang
* Description:
* Operate Elevator.h
*/

#pragma once

#include "CoreMinimal.h"
#include "Master_InteractableObject.h"
#include "InteractableObject_ElevatorBtn.generated.h"

/**
 * 
 */
UCLASS()
class CAP2_API AInteractableObject_ElevatorBtn : public AMaster_InteractableObject
{
	GENERATED_BODY()

public:
	AInteractableObject_ElevatorBtn();


	UPROPERTY(EditAnywhere)
		AActor* Elevator;

protected:
	virtual void Interact() override;
};
