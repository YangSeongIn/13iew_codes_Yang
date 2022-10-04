/*
* Header "Master_Movable"
* Created by Yang
* Last Modified by Yang
* Description:
* An object that can be pushed or pulled. Available in 3rd person only.
*/


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"

#include "Master_Movable.generated.h"

UCLASS()
class CAP2_API AMaster_Movable : public AActor
{
	GENERATED_BODY()
	
public:	
	AMaster_Movable();

	UPROPERTY()
		class UMyGameInstance* GameInstance;

	FVector DefaultBoxLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* Box;

	UPROPERTY()
		class UHighlightableComponent* HightlightableComponent;

	UFUNCTION()
		void Reset();
	UFUNCTION()
		void Save();
	UFUNCTION()
		void InitializeBox();

protected:
	virtual void BeginPlay() override;

};
