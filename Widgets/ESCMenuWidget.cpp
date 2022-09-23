#include "ESCMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "MyGameInstance.h"
#include "SaveGameObject.h"
#include "SettingWidget.h"


void UESCMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton->OnClicked.AddDynamic(this, &UESCMenuWidget::ResumeGame);
	RestartButton->OnClicked.AddDynamic(this, &UESCMenuWidget::RestartGame);
	QuitButton->OnClicked.AddDynamic(this, &UESCMenuWidget::QuitGame);

	// Park
	SettingButton->OnClicked.AddDynamic(this, &UESCMenuWidget::SetOption);
}

void UESCMenuWidget::ResumeGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(false);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	RemoveFromParent();
}

void UESCMenuWidget::RestartGame()
{
	ResumeGame();
	Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->LoadData();
}

void UESCMenuWidget::QuitGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("Main_Menu"));
}

void UESCMenuWidget::SetOption()
{
	RemoveFromParent();
	if (_SettingWidget)
	{
		SettingWidget = Cast<USettingWidget>(CreateWidget(GetWorld(), _SettingWidget));
		if (SettingWidget)
		{
			SettingWidget->AddToViewport();
			SettingWidget->ParentType = 1;
		}
	}
}