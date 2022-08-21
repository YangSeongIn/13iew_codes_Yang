// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.h"

#include "MainMenuLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class CAP2_API AMainMenuLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Widget")
		TSubclassOf<UUserWidget> _MainMenuWidget;
	UPROPERTY(VisibleInstanceOnly, Category = "Widget")
		UMainMenuWidget* MainMenuWidget;

protected:
	virtual void BeginPlay() override;
};
