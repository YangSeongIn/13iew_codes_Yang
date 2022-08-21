// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Master_InteractableObject.h"
#include "Interface_GaugeWidget.h"

#include "InteractableObject_Valve.generated.h"

UCLASS()
class CAP2_API AInteractableObject_Valve : public AMaster_InteractableObject
{
	GENERATED_BODY()

private:
	float ValveGauge = 0.0f;
	float ValveRotateValue = 0.0f;
	bool bIsInteracting;
	bool bCanInteract = true;
	bool bIsFinished;
	UPROPERTY(EditAnywhere)
		bool bPlayMusic;

	UPROPERTY(EditAnywhere)
		float ValveGaugeSpeed = 0.1f;
	float ValveRotationSpeed = 1;
	UPROPERTY(EditAnywhere)
		float ValveRotationCycleTime = 1;
	float ValveReverseSpeed = 3.5f;

	UPROPERTY(EditAnywhere)
		float LinkedActorSpeed = 0.9f;

public:
	AInteractableObject_Valve();

	class AMainCharacter* MainCharacter;

	UPROPERTY(EditAnywhere)
		AActor* LinkedActor;

	void LinkedActorAction(float Gauge);

	void StopInteract();
	void ProgressingGauge();
	void ReverseGauge();
	void ReverseGaugeEvent(float FSpeed);
	void ValveGaugeComplete();

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> _GaugeWidget;
	UPROPERTY()
		class UGaugeWidget* GaugeWidget;
	UPROPERTY(EditAnywhere)
		class ASoundManager* SoundManager;

	class UAudioComponent* HorrorAudio;

protected:
	virtual void Interact() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
