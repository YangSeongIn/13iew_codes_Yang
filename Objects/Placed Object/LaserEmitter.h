/*
* Header "LaserEmitter"
* Created by Yang
* Last Modified by Yang
* Description:
* Generate a laser. It is used to shoot the projector in the projector room on the 1st floor of Level 1.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystemComponent.h"

#include "LaserEmitter.generated.h"

UCLASS()
class CAP2_API ALaserEmitter : public AActor
{
	GENERATED_BODY()
	
public:	
	ALaserEmitter();

	int32 MaxReflectionNum = 10;
	UPROPERTY(EditAnywhere)
		float LaserStartPoint = 0.0f;

	void CastLight(FVector CastOrigin, FVector CastDirection);

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Cube;
	UMaterialInstance* MirrorMat;

	UPROPERTY(EditAnywhere)
		UParticleSystem* LaserEmitter;

public:	
	virtual void Tick(float DeltaTime) override;

};
