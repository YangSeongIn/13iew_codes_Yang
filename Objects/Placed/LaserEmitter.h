
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

	bool bDoOncePlay = true;

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

	UPROPERTY(EditAnywhere)
		class ULevelSequence* SequenceAsset;
	UPROPERTY()
		class ULevelSequencePlayer* SequencePlayer;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
