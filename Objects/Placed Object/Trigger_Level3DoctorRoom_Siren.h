#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/RectLightComponent.h"
#include "Trigger_Level3DoctorRoom_Siren.generated.h"

UCLASS()
class CAP2_API ATrigger_Level3DoctorRoom_Siren : public AActor
{
	GENERATED_BODY()
	
public:	
	ATrigger_Level3DoctorRoom_Siren();

protected:
	virtual void BeginPlay() override;

public:	
	bool bIsVisible = true;
	UPROPERTY(EditAnywhere)
		UBoxComponent* TriggerBox;
	UPROPERTY(EditAnywhere)
		URectLightComponent* RectLight;

	FTimerHandle TimerHandle_Delay;
	FTimerDelegate Delegate;

	UPROPERTY(EditAnywhere)
		float TimeDelay;

	UFUNCTION()
		void ActivateRectLight();

	UFUNCTION()
		void OnCapsuleComponentOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnCapsuleComponentOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
