/*
* Header "Master_Pole"
* Created by Yang
* Last Modified by Yang
* Description:
* A pole that can go up and down. Available in 3rd person only.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"

#include "Master_Pole.generated.h"

UCLASS()
class CAP2_API AMaster_Pole : public AActor
{
	GENERATED_BODY()
	
public:	
	AMaster_Pole();

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		USceneComponent* DefaultSceneRoot;
	UPROPERTY(EditAnywhere)
		UBoxComponent* Box;

	class AMainCharacter* MainCharacter;
	UPROPERTY()
		class UHighlightableComponent* HightlightableComponent;
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
