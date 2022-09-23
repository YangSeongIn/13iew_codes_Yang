/*
* Header "Master_InteratableObject"
* Created by Yang
* Last Modified by Yang
* Description:
* The parent class of items that can be used in first person Mode.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

#include "Master_InteractableItem.generated.h"

UCLASS()
class CAP2_API AMaster_InteractableItem : public AActor
{
	GENERATED_BODY()

public:
	AMaster_InteractableItem();
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		USceneComponent* _RootComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* StaticMesh;
	UPROPERTY()
		class UHighlightableComponent* HightlightableComponent;

public:
	virtual void Interact() PURE_VIRTUAL(AMaster_InteractableObject::Interact, );
};
