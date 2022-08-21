// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Attention.generated.h"

UCLASS()
class CAP2_API AAttention : public AActor
{
	GENERATED_BODY()
	
public:	
	AAttention();

	class AMainCharacter* MainCharacter;

	UPROPERTY(EditAnywhere)
		float SphereSize;

public:	
	virtual void Tick(float DeltaTime) override;
};
