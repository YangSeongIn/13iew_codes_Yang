/*
* Header "ESCMenuWidget"
* Created by Yang
* Last Modified by Park
* Description:
* menu that appears when user press the esc key.
*/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ESCMenuWidget.generated.h"

UCLASS()
class CAP2_API UESCMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
		class UButton* ResumeButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* RestartButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* SettingButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* QuitButton;

	UFUNCTION()
		void ResumeGame();
	UFUNCTION()
		void RestartGame();
	UFUNCTION()
		void QuitGame();

	// Park
	UFUNCTION()
		void SetOption();
	UPROPERTY()
		class USettingWidget* SettingWidget;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> _SettingWidget;

protected:
	virtual void NativeConstruct() override;
};
