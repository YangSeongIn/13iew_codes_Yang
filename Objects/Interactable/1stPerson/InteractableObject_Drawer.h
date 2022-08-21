// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Master_InteractableObject.h"
#include "InteractableItem_Key.h"
#include "Components/TimelineComponent.h"

#include "InteractableObject_Drawer.generated.h"

class AMainCharacter;
UCLASS()
class CAP2_API AInteractableObject_Drawer : public AMaster_InteractableObject
{
	GENERATED_BODY()

private:
	bool bIsClosed = true;
public:
	AInteractableObject_Drawer();

	UFUNCTION()
		void OnFinish();

	UFUNCTION()
		void InterpReturn(float value);

	FOnTimelineFloat InterpFunction;
	FOnTimelineEvent TimelineFinish;

	UPROPERTY()
		UTimelineComponent* DrawerTimeline;

	UPROPERTY(EditAnywhere)
		UCurveFloat* DrawerCurveFloat;

	//UPROPERTY(EditAnywhere)
	//	AInteractableItem_Key* Key;

	AMainCharacter* InteractCharacter;
	UPROPERTY(EditAnywhere)
		class ASoundManager* SoundManager;

	// Delegate
	class UMyGameInstance* GameInstance;
	UFUNCTION()
		void Reset();
	UFUNCTION()
		void Save();

public:
	virtual void BeginPlay() override;
	virtual void Interact() override;
};