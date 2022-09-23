/*
* Header "Master_InteratableObject"
* Created by Yang
* Last Modified by Yang
* Description:
* The parent class of objects that can be used in first person Mode.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Master_InteractableObject.generated.h"

UCLASS()
class CAP2_API AMaster_InteractableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	AMaster_InteractableObject();
	UPROPERTY(BlueprintReadOnly)
		USceneComponent* _RootComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* StaticMesh;
	UPROPERTY()
		class UHighlightableComponent* HightlightableComponent;
public:	
	UFUNCTION()
		virtual void Interact() PURE_VIRTUAL(AMaster_InteractableObject::Interact, );
};
