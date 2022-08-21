// Fill out your copyright notice in the Description page of Project Settings.

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

protected:
	virtual void BeginPlay() override;

};
