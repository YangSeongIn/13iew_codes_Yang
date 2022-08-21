// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainCharacterStructs.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FMainCharacterStruct
{
	GENERATED_BODY()

public:
	//UPROPERTY()
		//FVector MainCharacterLocation = FVector(1074.279053f, 1689.859375f, 1396.703125f);	// Movable Area
		//FVector MainCharacterLocation = FVector(1164.279297f, 207.863281f, 2374.703125f);
		//FVector MainCharacterLocation = FVector(966.0f, -2210.0f, 3836.0f);	// Door Area
	UPROPERTY()
		bool bIsStartingPoint = true;
};