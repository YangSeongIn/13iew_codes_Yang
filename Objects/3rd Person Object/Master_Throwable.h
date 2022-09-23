/*
* Header "Master_Throwable"
* Created by Yang
* Last Modified by Yang
* Description:
* An object that can be lifted or thrown. Can be thrown with the space bar key. Available in 3rd person only.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "Components/BoxComponent.h"
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

	UPROPERTY(EditAnywhere)
		UBoxComponent* BoxComponent;

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

	AActor* HitActor;

	UFUNCTION(BlueprintCallable)
		void HitSound(FVector Location);
	UPROPERTY(EditAnywhere)
		class USoundCue* HitSoundCue;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
protected:
	virtual void BeginPlay() override;
};
