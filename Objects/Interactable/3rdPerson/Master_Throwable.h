// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "Master_Throwable.generated.h"

UCLASS()
class CAP2_API AMaster_Throwable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMaster_Throwable();
	
	class UMyGameInstance* GameInstance;

	UPROPERTY(EditAnywhere)
		float ThrowPowerFloat = 2000;

	UPROPERTY()
		bool bIsAttached;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMesh;

	UFUNCTION()
		void MoveComponentToFunction();

	UPROPERTY()
		class UHighlightableComponent* HightlightableComponent;

	UFUNCTION()
		void Reset();
	UFUNCTION()
		void Save();

	USoundCue* DistractSoundCue;
	bool isHit = false;
	// AI
	class UAIPerceptionStimuliSourceComponent* stimulus;
	void SetupStimulus();

public:
	void DropDown();
	void PickUp(FVector PickUpLocation, class AMainCharacter* MainCharacter);
	void Throw(class AMainCharacter* MainCharacter, float ChargeRate);

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	virtual void BeginPlay() override;
};
