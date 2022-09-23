#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "DestructibleComponent.h"

#include "Trigger_FloorBreak.generated.h"

UCLASS()
class CAP2_API ATrigger_FloorBreak : public AActor
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:	
	ATrigger_FloorBreak();

	UPROPERTY(EditAnywhere)
		UBoxComponent* TriggerBox;

	UFUNCTION()
		void OnCapsuleComponentOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnCapsuleComponentOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


};
