// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Master_InteractableObject.h"
#include "InteractableItem_Key.h"
#include "Interface_InteractWithWidget.h"
#include "Components/TimelineComponent.h"
#include "FirstpersonDefaultWidget.h"
#include "Components/BoxComponent.h"
#include "InteractableObject_Door.generated.h"

/**
 *
 */
class AMainCharacter;

UCLASS()
class CAP2_API AInteractableObject_Door : public AMaster_InteractableObject, public IInterface_InteractWithWidget
{
	GENERATED_BODY()

private:
	bool bIsFront;
	bool bIsOpeningFront;

	float f;

	int32 DoN = 0;

public:
	AInteractableObject_Door();

	bool bIsClosed = true;

	bool GetDoorIsOpened() { return !bIsClosed; };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bIsFrontDoor = false;

	UFUNCTION()
		void OnFinish();

	UFUNCTION()
		void InterpReturn(float value);

	FOnTimelineFloat InterpFunction;
	FOnTimelineEvent TimelineFinish;

	UPROPERTY(EditAnywhere)
		USceneComponent* DefaultSceneRoot;
	UPROPERTY()
		UTimelineComponent* DoorTimeline;

	UPROPERTY(EditAnywhere)
		UCurveFloat* DoorCurveFloat;

	AMainCharacter* InteractCharacter;

	UPROPERTY(EditAnywhere)
		bool bUseKey;
	UPROPERTY(EditAnywhere)
		AInteractableItem_Key* Key;

	UPROPERTY(EditAnywhere)
		class ASoundManager* SoundManager;

	// Widget
	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> DoorWidgetClass;
	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
		UFirstPersonDefaultWidget* FirstPersonDoorWidget;

	UFUNCTION(BlueprintCallable)
		void DelayFunc();

	FLatentActionInfo LAI;

	// Delegate
	class UMyGameInstance* GameInstance;
	UFUNCTION()
		void Reset();
	UFUNCTION()
		void Save();

	//YK
	void OpenForAI();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBoxComponent* BoxComponent;
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
protected:
	virtual void BeginPlay() override;
	virtual void Interact() override;
	virtual void OnFocus() override;
};