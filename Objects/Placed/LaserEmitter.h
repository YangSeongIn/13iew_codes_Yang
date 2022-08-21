// Fill out your copyright notice in the Description page of Project Settings.

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
	// Sets default values for this actor's properties
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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
