// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuLevelScriptActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void AMainMenuLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();
	if (_MainMenuWidget)
	{
		MainMenuWidget = Cast<UMainMenuWidget>(CreateWidget(GetWorld(), _MainMenuWidget));
		if (MainMenuWidget)
		{
			MainMenuWidget->AddToViewport();
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;
		}
	}	
}