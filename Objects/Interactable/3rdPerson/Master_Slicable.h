/*
* Header "Master_Slicable"
* Created by Park
* Last Modified by Park
* Modified: 2022.05.15
* Description: Slicable Object Can be Sliced or Droped
*/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Level1Structs.h"

#include "Master_Slicable.generated.h"

UCLASS()
class CAP2_API AMaster_Slicable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMaster_Slicable();

public:	
	void SliceMesh(FVector);
	void DropMesh(FVector);

	bool bIsUsed;

	UPROPERTY()
		class UMyGameInstance* GameInstance;
	UFUNCTION()
		void Reset();
	UFUNCTION()
		void Save();
	FSlicableStruct SlicableStruct;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* SlicableScene;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* SlicableMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UProceduralMeshComponent* ProceduralMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UProceduralMeshComponent* OutHalfMesh;
	UPROPERTY(EditAnywhere)
		UBoxComponent* Box;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* HandIKLoc;

	UPROPERTY()
		class UHighlightableComponent* HightlightableComponent;

	class AMainCharacter* MainCharacter;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	virtual void BeginPlay() override;
};
