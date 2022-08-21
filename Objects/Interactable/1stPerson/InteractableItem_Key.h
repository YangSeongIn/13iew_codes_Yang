// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Master_InteractableItem.h"
#include "InteractableItem_Key.generated.h"

/**
 * 
 */
UCLASS()
class CAP2_API AInteractableItem_Key : public AMaster_InteractableItem
{
	GENERATED_BODY()

public:
	AInteractableItem_Key();
	
	class AMainCharacter* MainCharacter;

public:
	virtual void BeginPlay() override;
	virtual void Interact() override;
};
