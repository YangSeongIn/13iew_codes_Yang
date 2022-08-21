// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MultiSlotWidget.h"
#include "SettingWidget.h"
#include "SlotWidget.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, TEXT("ASDF"));

	PlayButton->SlotIndex = -1;
	PlayButton->SlotName = FString("Play");
	SettingButton->SlotIndex = -1;
	SettingButton->SlotName = FString("Setting");
	CreditButton->SlotIndex = -1;
	CreditButton->SlotName = FString("Credit");
	QuitButton->SlotIndex = -1;
	QuitButton->SlotName = FString("Quit");

	// Park
	//SettingButton->OnClicked.AddDynamic(this, &UMainMenuWidget::SetOption);
}

void UMainMenuWidget::SetOption()
{
	RemoveFromParent();
	if (_SettingWidget)
	{
		SettingWidget = Cast<USettingWidget>(CreateWidget(GetWorld(), _SettingWidget));
		if (SettingWidget)
		{
			SettingWidget->AddToViewport();
		}
	}
	else
	{

	}
}