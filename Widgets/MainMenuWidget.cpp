#include "MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MultiSlotWidget.h"
#include "SettingWidget.h"
#include "SlotWidget.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayButton->SlotIndex = -1;
	PlayButton->SlotName = FString("Play");
	SettingButton->SlotIndex = -1;
	SettingButton->SlotName = FString("Setting");
	CreditButton->SlotIndex = -1;
	CreditButton->SlotName = FString("Credit");
	QuitButton->SlotIndex = -1;
	QuitButton->SlotName = FString("Quit");
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
}