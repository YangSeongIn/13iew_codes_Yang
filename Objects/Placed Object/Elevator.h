/*
* Header "Elevator"
* Created by Yang
* Last Modified by Yang
* Description:
* Gimmick at the last point on the 2nd floor of Level 2.
*/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Elevator.generated.h"

UCLASS()
class CAP2_API AElevator : public AActor
{
	GENERATED_BODY()
	
public:	
	AElevator();
	UPROPERTY(EditAnywhere)
		float DoorMoveLength = 260.0f;
	UPROPERTY(EditAnywhere)
		float ElevatorMoveLength = 500.0f;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Frame;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Door;

	UFUNCTION()
		void Operate(class AInteractableObject_ElevatorBtn* Btn);
	UFUNCTION()
		void Elevate();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
