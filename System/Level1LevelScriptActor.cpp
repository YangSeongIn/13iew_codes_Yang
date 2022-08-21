// Fill out your copyright notice in the Description page of Project Settings.


#include "Level1LevelScriptActor.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameState.h"
#include "MyGameInstance.h"
#include "Master_Movable.h"
#include "MainCharacter.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/BoxComponent.h"

void ALevel1LevelScriptActor::BeginPlay()
{
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(false);

	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (MainCharacter)
		MainCharacter->ResetHidedActors();
}