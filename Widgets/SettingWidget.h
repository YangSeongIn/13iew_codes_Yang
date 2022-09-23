/*
* Header "SettingWidget"
* Created by Park
* Last Modified by Park
* Modified: 2022.08.03
* Description: Setting Widget
*/
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Slider.h"
#include "GameFramework/GameUserSettings.h"
#include "Components/TextBlock.h"
#include "SettingWidget.generated.h"

UCLASS()
class CAP2_API USettingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		class USlider* MasterVolumeSlider;
	UPROPERTY(meta = (BindWidget))
		class USlider* EffectVolumeSlider;
	UPROPERTY(meta = (BindWidget))
		class USlider* MouseSensitivitySlider;
	UPROPERTY(meta = (BindWidget))
		class UButton* BT_Resolution_3840_2160;
	UPROPERTY(meta = (BindWidget))
		class UButton* BT_Resolution_2560_1440;
	UPROPERTY(meta = (BindWidget))
		class UButton* BT_Resolution_1920_1080;
	UPROPERTY(meta = (BindWidget))
		class UButton* BT_Resolution_1336_768;
	UPROPERTY(meta = (BindWidget))
		class UButton* BT_Resolution_1280_720;
	UPROPERTY(meta = (BindWidget))
		class UButton* BT_Back;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> _ESCMenuWidget;
	UPROPERTY()
		class UESCMenuWidget* ESCMenuWidget;

	UFUNCTION()
		void OnMasterVolumeChanged(float ChangedValue);
	UFUNCTION()
		void OnEffectVolumeChanged(float ChangedValue);
	UFUNCTION()
		void OnMouseSensitivityChanged(float ChangedValue);
	UFUNCTION()
		void SetResolution_3840_2160();
	UFUNCTION()
		void SetResolution_2560_1440();
	UFUNCTION()
		void SetResolution_1920_1080();
	UFUNCTION()
		void SetResolution_1336_768();
	UFUNCTION()
		void SetResolution_1280_720();

	float GetMasterVolume();
	float GetEffectVolume();
	float GetMouseSensitivity();
	float MasterVolumeValue;
	float EffectVolumeValue;
	float MouseSensitivityValue;

	// VSync
	UPROPERTY(meta = (BindWidget))
		class UButton* BT_VSyncOff;
	UFUNCTION()
		void SetVSyncOff();
	UPROPERTY(meta = (BindWidget))
		class UButton* BT_VSyncOn;
	UFUNCTION()
		void SetVSyncOn();

	// Graphic
	UPROPERTY(meta = (BindWidget))
		class UButton* BT_GraphicLow;
	UPROPERTY(meta = (BindWidget))
		class UButton* BT_GraphicMid;
	UPROPERTY(meta = (BindWidget))
		class UButton* BT_GraphicHigh;
	UPROPERTY(meta = (BindWidget))
		class UButton* BT_GraphicMax;
	UFUNCTION()
		void SetGraphicLow();
	UFUNCTION()
		void SetGraphicMid();
	UFUNCTION()
		void SetGraphicHigh();
	UFUNCTION()
		void SetGraphicMax();

	UFUNCTION()
		void SetButtonEnabled();

	// Detail
	UPROPERTY(meta = (BindWidget))
		class UButton* BT_GraphicCustom;	// DetailSetting
	UFUNCTION()
		void SetDetailOption();
	UPROPERTY()
		class UDetailSettingWidget* DetailSettingWidget;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> _DetailSettingWidget;
	
	// Manager
	UPROPERTY(EditAnywhere)
		class ATestSettingActor* _SettingManager;


	int ParentType = 0;	//0:MainMenu, 1:ESC

	// Highlight Clicked Button
	//----------
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextResolution1280;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextResolution1336;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextResolution1920;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextResolution2560;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextResolution3840;
	//----------
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextVSyncOn;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextVSyncOff;
	//----------
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextGraphicLow;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextGraphicMid;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextGraphicHigh;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextGraphicMax;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextGraphicCustom;

	void SetButtonClicked(UTextBlock* Text, bool b);
	void HighlightButtons();


	UPROPERTY(meta = (BindWidget))
		class UTextBlock* BackButtonText;
	UFUNCTION()
		void OnHovered();
	UFUNCTION()
		void OnUnHovered();
	UFUNCTION()
		void BackToESCMenuWidget();

protected:
	virtual void NativeConstruct() override;
	
};
