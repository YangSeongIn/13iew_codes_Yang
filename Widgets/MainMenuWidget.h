/*
* Header "MainMenuWidget"
* Created by Yang
* Last Modified by Park
* Description:
* Main Menu Widget.
*/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "MainMenuWidget.generated.h"

DECLARE_DELEGATE(FOnClickStartGame);

UCLASS()
class CAP2_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		class USlotWidget* PlayButton;
	UPROPERTY(meta = (BindWidget))
		class USlotWidget* SettingButton;
		//class UButton* SettingButton;
	UPROPERTY(meta = (BindWidget))
		class USlotWidget* CreditButton;
	UPROPERTY(meta = (BindWidget))
		class USlotWidget* QuitButton;

	UFUNCTION()
		void SetOption();

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> _MultiSlotWidget;
	UPROPERTY()
		class UMultiSlotWidget* MultiSlotWidget;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> _SettingWidget;
	UPROPERTY()
		class USettingWidget* SettingWidget;

	FOnClickStartGame OnClickStartGame;

	// Park

protected:
	virtual void NativeConstruct() override;
};
