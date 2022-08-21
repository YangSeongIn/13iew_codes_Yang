// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeneratorUsingTimer.generated.h"

UCLASS()
class CAP2_API AGeneratorUsingTimer : public AActor
{
	GENERATED_BODY()

private:
	bool bIsGeneratingCorpse;

public:	
	AGeneratorUsingTimer();

	bool bIsOnCorpseGenerator;

	FTimerHandle TimerHandle_AttackDelay;
	FTimerDelegate Delegate;

	UPROPERTY(EditAnywhere)
		float SpawnTimeDelay;
	UFUNCTION(BlueprintCallable)
		void ActivateCorpseGenerator();
	UFUNCTION()
		void GenerateCorpse(class AMaster_Corpse* C);
	UFUNCTION(BlueprintCallable)
		void DeActivateCorpseGenerator();

	UPROPERTY()
		class UMyGameInstance* GameInstance;
	UFUNCTION()
		void Reset();
	UFUNCTION()
		void Save();

protected:
	virtual void BeginPlay() override;
};
