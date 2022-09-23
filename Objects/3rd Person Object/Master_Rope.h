/*
* Header "Master_Rope"
* Created by Yang
* Last Modified by Yang
* Description:
* A rope that can be suspended or swung while suspended. Available in 3rd person only.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CableComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/CapsuleComponent.h"
#include "Interface_MainCharacterInfo.h"
#include "Kismet/KismetMathLibrary.h"

#include "Master_Rope.generated.h"

UCLASS()
class CAP2_API AMaster_Rope : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMaster_Rope();

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		bool bIsOneSideRope;
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		float HangingLocZ;
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		float bIsAbleToSwingZ;
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		USceneComponent* DefaultSceneRoot;
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		UCableComponent* Cable;
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		UStaticMeshComponent* CableEnd;
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		UPhysicsConstraintComponent* PhysicsConstraint;
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMesh;

	IInterface_MainCharacterInfo* Interface_MainCharacterInfo;

	UPROPERTY()
		class UHighlightableComponent* HightlightableComponent;

	// Collision
	UFUNCTION()
		void OnCapsuleComponentOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnCapsuleComponentOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
