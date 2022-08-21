// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HighlightableComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CAP2_API UHighlightableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHighlightableComponent();

	bool bIsOnFocus;
	int8 DoN = 0;

	class AMainCharacter* MainCharacter;

	UStaticMeshComponent* StaticMesh;
	class UCableComponent* Cable;

	UFUNCTION()
		void TurnOnHighlight();
	
	FLatentActionInfo LAI;

	UFUNCTION()
		void DelayFunc();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
