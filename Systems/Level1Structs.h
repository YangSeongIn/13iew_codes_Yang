/*
* Header "Level1Structs"
* Created by Yang
* Last Modified by Yang
* Description:
* Struct to store level1 information. To be merged into one soon.
*/

#pragma once

#include "CoreMinimal.h"

#include "Level1Structs.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FSlicableStruct
{
	GENERATED_BODY()

public:
	FTransform OutHalfMeshTransform;
	FTransform ProceduralMeshTransform;
	FTransform SlicableMeshTransform;
	bool bIsUsed;
};

USTRUCT(Atomic, BlueprintType)
struct FObjectToBeSavedStruct
{
	GENERATED_BODY()

public:
	bool bIsTransform;
	bool bIsVisibility;
};

USTRUCT(Atomic, BlueprintType)
struct FLevel1Struct
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
		TMap<class AInteractableObject_Levor*, bool> LeverData;
	UPROPERTY()
		TMap<class AMaster_Slicable*, bool> SlicableData;
	UPROPERTY()
		TMap<class AConveyorBelt*, bool> ConveyorBeltData;
	UPROPERTY()
		TMap<class AGeneratorUsingTimer*, bool> CorpseGeneratorData;
	UPROPERTY()
		TMap<class AObjectToBeSaved*, bool> ObjectToBeSavedData_Transform;
	UPROPERTY()
		TMap<class AObjectToBeSaved*, bool> ObjectToBeSavedData_Visibility;
	UPROPERTY()
		TMap<class AMaster_Throwable*, FTransform> ThrowableData;

	UPROPERTY()
		bool bFlashLightIsDestroyed;
	UPROPERTY()
		FVector MainCharacterLocation = FVector(1074.279053f,  2003.859375f, 1396.703125f);
	// record version
	//UPROPERTY()
	//	FVector MainCharacterLocation = FVector(-3931, 10314, 87);
};
