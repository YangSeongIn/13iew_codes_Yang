// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Level2Structs.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FLevel2Struct
{
	GENERATED_BODY()

public:
	UPROPERTY()
		TMap<class ACheckPoint*, bool> CheckPointData;
	UPROPERTY()
		TMap<class AMaster_Movable*, FTransform> MovableData;
	UPROPERTY()
		TMap<class AInteractableObject_Door*, bool> DoorData;
	UPROPERTY()
		TMap<class AMaster_Throwable*, FTransform> ThrowableData;
	UPROPERTY()
		TMap<class AObjectToBeSaved*, bool> ObjectToBeSavedData_Transform;
	UPROPERTY()
		TMap<class AObjectToBeSaved*, bool> ObjectToBeSavedData_Visibility;

	UPROPERTY()
		FVector MainCharacterLocation = FVector(-527.347290, 99.627319, 182.078003); // Entrance
		// FVector MainCharacterLocation = FVector(4576.652832, 4159.627441, 242.078003); // Balance
		//FVector MainCharacterLocation = FVector(4823, 9771, 107); // Projector Room
};
