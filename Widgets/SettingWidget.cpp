/*
* Source "SettingWidget"
* Created by Park
* Last Modified by Park
* Modified: 2022.08.03
* Description: Setting Widget
*/

#include "SettingWidget.h"
#include "Components/Button.h"
#include "ESCMenuWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "TestSettingActor.h"
#include "DetailSettingWidget.h"

void USettingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BT_Back = Cast<UButton>(GetWidgetFromName(TEXT("BT_Back")));
	BT_Back->OnHovered.AddDynamic(this, &USettingWidget::OnHovered);
	BT_Back->OnUnhovered.AddDynamic(this, &USettingWidget::OnUnHovered);
	BT_Back->OnClicked.AddDynamic(this, &USettingWidget::BackToESCMenuWidget);

	BT_Resolution_3840_2160 = Cast<UButton>(GetWidgetFromName(TEXT("BT_Resolution_3840_2160")));
	BT_Resolution_2560_1440 = Cast<UButton>(GetWidgetFromName(TEXT("BT_Resolution_2560_1440")));
	BT_Resolution_1920_1080 = Cast<UButton>(GetWidgetFromName(TEXT("BT_Resolution_1920_1080")));
	BT_Resolution_1336_768 = Cast<UButton>(GetWidgetFromName(TEXT("BT_Resolution_1336_768")));
	BT_Resolution_1280_720 = Cast<UButton>(GetWidgetFromName(TEXT("BT_Resolution_1280_720")));

	BT_Resolution_3840_2160->OnClicked.AddDynamic(this, &USettingWidget::SetResolution_3840_2160);
	BT_Resolution_2560_1440->OnClicked.AddDynamic(this, &USettingWidget::SetResolution_2560_1440);
	BT_Resolution_1920_1080->OnClicked.AddDynamic(this, &USettingWidget::SetResolution_1920_1080);
	BT_Resolution_1336_768->OnClicked.AddDynamic(this, &USettingWidget::SetResolution_1336_768);
	BT_Resolution_1280_720->OnClicked.AddDynamic(this, &USettingWidget::SetResolution_1280_720);

	MasterVolumeSlider->OnValueChanged.AddDynamic(this, &USettingWidget::OnMasterVolumeChanged);
	EffectVolumeSlider->OnValueChanged.AddDynamic(this, &USettingWidget::OnEffectVolumeChanged);
	MouseSensitivitySlider->OnValueChanged.AddDynamic(this, &USettingWidget::OnMouseSensitivityChanged);

	_SettingManager = Cast<ATestSettingActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ATestSettingActor::StaticClass()));

	// Volume and Mouse
	MasterVolumeValue = GetMasterVolume();
	MasterVolumeSlider->SetValue(MasterVolumeValue);
	EffectVolumeValue = GetEffectVolume();
	EffectVolumeSlider->SetValue(EffectVolumeValue);
	MouseSensitivityValue = GetMouseSensitivity();
	MouseSensitivitySlider->SetValue(MouseSensitivityValue);

	// VSync
	BT_VSyncOff = Cast<UButton>(GetWidgetFromName(TEXT("BT_VSyncOff")));
	BT_VSyncOff->OnClicked.AddDynamic(this, &USettingWidget::SetVSyncOff);
	BT_VSyncOn = Cast<UButton>(GetWidgetFromName(TEXT("BT_VSyncOn")));
	BT_VSyncOn->OnClicked.AddDynamic(this, &USettingWidget::SetVSyncOn);
	// Graphic
	BT_GraphicLow = Cast<UButton>(GetWidgetFromName(TEXT("BT_GraphicLow")));
	BT_GraphicLow->OnClicked.AddDynamic(this, &USettingWidget::SetGraphicLow);
	BT_GraphicMid = Cast<UButton>(GetWidgetFromName(TEXT("BT_GraphicMid")));
	BT_GraphicMid->OnClicked.AddDynamic(this, &USettingWidget::SetGraphicMid);
	BT_GraphicHigh = Cast<UButton>(GetWidgetFromName(TEXT("BT_GraphicHigh")));
	BT_GraphicHigh->OnClicked.AddDynamic(this, &USettingWidget::SetGraphicHigh);
	BT_GraphicMax = Cast<UButton>(GetWidgetFromName(TEXT("BT_GraphicMax")));
	BT_GraphicMax->OnClicked.AddDynamic(this, &USettingWidget::SetGraphicMax);
	// Detail
	BT_GraphicCustom = Cast<UButton>(GetWidgetFromName(TEXT("BT_GraphicCustom")));
	BT_GraphicCustom->OnClicked.AddDynamic(this, &USettingWidget::SetDetailOption);

	SetButtonEnabled();
	HighlightButtons();
}


void USettingWidget::SetButtonEnabled()
{
	if (_SettingManager)
	{
		ATestSettingActor* SettingManager = Cast<ATestSettingActor>(_SettingManager);
		FIntPoint MaximumResolution = SettingManager->GetDesktopResolution();

		// Resolution Button Enabled Check
		UButton* ResolutionButtonArr[] = { BT_Resolution_1280_720, BT_Resolution_1336_768, BT_Resolution_1920_1080, BT_Resolution_2560_1440, BT_Resolution_3840_2160 };
		FIntPoint ResolutionSizeArr[] = { FIntPoint(1280, 720), FIntPoint(1336, 768), FIntPoint(1920, 1080), FIntPoint(2560, 1440), FIntPoint(3840, 2160) };
		int ArrSize = sizeof(ResolutionButtonArr) / sizeof(ResolutionButtonArr[0]);

		for (int i = 0; i < ArrSize; i++)
		{
			if (ResolutionSizeArr[i].X <= MaximumResolution.X && ResolutionSizeArr[i].Y <= MaximumResolution.Y)
			{
				// Button Enable
			}
			else
			{
				ResolutionButtonArr[i]->SetIsEnabled(false);
			}
		}
	}
}

//------------------------------------------------------------
void USettingWidget::SetDetailOption()
{
	RemoveFromParent();
	if (_DetailSettingWidget)
	{
		DetailSettingWidget = Cast<UDetailSettingWidget>(CreateWidget(GetWorld(), _DetailSettingWidget));
		if (DetailSettingWidget)
		{
			DetailSettingWidget->AddToViewport();
		}
	}
}

//------------------------------------------------------------
void USettingWidget::SetVSyncOn()
{
	if (_SettingManager)
	{
		ATestSettingActor* SettingManager = Cast<ATestSettingActor>(_SettingManager);
		SettingManager->SetVSyncEnabled(true);

		SetButtonClicked(TextVSyncOn, true);
		SetButtonClicked(TextVSyncOff, false);
	}

}

void USettingWidget::SetVSyncOff()
{
	if (_SettingManager)
	{
		ATestSettingActor* SettingManager = Cast<ATestSettingActor>(_SettingManager);
		SettingManager->SetVSyncEnabled(false);

		SetButtonClicked(TextVSyncOn, false);
		SetButtonClicked(TextVSyncOff, true);
	}
}

float USettingWidget::GetMasterVolume()
{
	if (_SettingManager)
	{
		ATestSettingActor* SettingManager = Cast<ATestSettingActor>(_SettingManager);
		return SettingManager->GetMasterVolume();
	}
	else
	{
		return 0.5f;
	}
}

float USettingWidget::GetEffectVolume()
{
	if (_SettingManager)
	{
		ATestSettingActor* SettingManager = Cast<ATestSettingActor>(_SettingManager);
		return SettingManager->GetEffectVolume();
	}
	else
		return 0.5f;
}

float USettingWidget::GetMouseSensitivity()
{
	if (_SettingManager)
	{
		ATestSettingActor* SettingManager = Cast<ATestSettingActor>(_SettingManager);
		return SettingManager->GetMouseSensitivity();
	}
	else
		return 1.0f;
}

//------------------------------------------------------------
void USettingWidget::SetResolution_3840_2160()
{
	if (_SettingManager)
	{
		ATestSettingActor* SettingManager = Cast<ATestSettingActor>(_SettingManager);
		SettingManager->SetWindowSize(5);

		SetButtonClicked(TextResolution1280, false);
		SetButtonClicked(TextResolution1336, false);
		SetButtonClicked(TextResolution1920, false);
		SetButtonClicked(TextResolution2560, false);
		SetButtonClicked(TextResolution3840, true);
	}
}

void USettingWidget::SetResolution_2560_1440()
{
	if (_SettingManager)
	{
		ATestSettingActor* SettingManager = Cast<ATestSettingActor>(_SettingManager);
		SettingManager->SetWindowSize(4);

		SetButtonClicked(TextResolution1280, false);
		SetButtonClicked(TextResolution1336, false);
		SetButtonClicked(TextResolution1920, false);
		SetButtonClicked(TextResolution2560, true);
		SetButtonClicked(TextResolution3840, false);
	}
}

void USettingWidget::SetResolution_1920_1080()
{
	if (_SettingManager)
	{
		ATestSettingActor* SettingManager = Cast<ATestSettingActor>(_SettingManager);
		SettingManager->SetWindowSize(3);

		SetButtonClicked(TextResolution1280, false);
		SetButtonClicked(TextResolution1336, false);
		SetButtonClicked(TextResolution1920, true);
		SetButtonClicked(TextResolution2560, false);
		SetButtonClicked(TextResolution3840, false);
	}
}

void USettingWidget::SetResolution_1336_768()
{
	if (_SettingManager)
	{
		ATestSettingActor* SettingManager = Cast<ATestSettingActor>(_SettingManager);
		SettingManager->SetWindowSize(2);

		SetButtonClicked(TextResolution1280, false);
		SetButtonClicked(TextResolution1336, true);
		SetButtonClicked(TextResolution1920, false);
		SetButtonClicked(TextResolution2560, false);
		SetButtonClicked(TextResolution3840, false);
	}
}

void USettingWidget::SetResolution_1280_720()
{
	if (_SettingManager)
	{
		ATestSettingActor* SettingManager = Cast<ATestSettingActor>(_SettingManager);
		SettingManager->SetWindowSize(1);

		SetButtonClicked(TextResolution1280, true);
		SetButtonClicked(TextResolution1336, false);
		SetButtonClicked(TextResolution1920, false);
		SetButtonClicked(TextResolution2560, false);
		SetButtonClicked(TextResolution3840, false);
	}
}

//------------------------------------------------------------
void USettingWidget::SetGraphicLow()
{
	if (_SettingManager)
	{
		ATestSettingActor* SettingManager = Cast<ATestSettingActor>(_SettingManager);
		SettingManager->SetAntiAliasingQuality(0);
		SettingManager->SetPostProcessingQuality(0);
		SettingManager->SetShadowQuality(0);
		SettingManager->SetTextureQuality(0);
		SettingManager->SetEffectQuality(0);
		SettingManager->SetAudioLevel(8);

		SetButtonClicked(TextGraphicLow, true);
		SetButtonClicked(TextGraphicMid, false);
		SetButtonClicked(TextGraphicHigh, false);
		SetButtonClicked(TextGraphicMax, false);
		SetButtonClicked(TextGraphicCustom, false);
	}
}

void USettingWidget::SetGraphicMid()
{
	if (_SettingManager)
	{
		ATestSettingActor* SettingManager = Cast<ATestSettingActor>(_SettingManager);
		SettingManager->SetAntiAliasingQuality(1);
		SettingManager->SetPostProcessingQuality(1);
		SettingManager->SetShadowQuality(1);
		SettingManager->SetTextureQuality(1);
		SettingManager->SetEffectQuality(1);
		SettingManager->SetAudioLevel(16);

		SetButtonClicked(TextGraphicLow, false);
		SetButtonClicked(TextGraphicMid, true);
		SetButtonClicked(TextGraphicHigh, false);
		SetButtonClicked(TextGraphicMax, false);
		SetButtonClicked(TextGraphicCustom, false);
	}
}

void USettingWidget::SetGraphicHigh()
{
	if (_SettingManager)
	{
		ATestSettingActor* SettingManager = Cast<ATestSettingActor>(_SettingManager);
		SettingManager->SetAntiAliasingQuality(2);
		SettingManager->SetPostProcessingQuality(2);
		SettingManager->SetShadowQuality(2);
		SettingManager->SetTextureQuality(2);
		SettingManager->SetEffectQuality(2);
		SettingManager->SetAudioLevel(32);

		SetButtonClicked(TextGraphicLow, false);
		SetButtonClicked(TextGraphicMid, false);
		SetButtonClicked(TextGraphicHigh, true);
		SetButtonClicked(TextGraphicMax, false);
		SetButtonClicked(TextGraphicCustom, false);
	}
}

void USettingWidget::SetGraphicMax()
{
	if (_SettingManager)
	{
		ATestSettingActor* SettingManager = Cast<ATestSettingActor>(_SettingManager);
		SettingManager->SetAntiAliasingQuality(3);
		SettingManager->SetPostProcessingQuality(3);
		SettingManager->SetShadowQuality(3);
		SettingManager->SetTextureQuality(3);
		SettingManager->SetEffectQuality(3);
		SettingManager->SetAudioLevel(32);

		SetButtonClicked(TextGraphicLow, false);
		SetButtonClicked(TextGraphicMid, false);
		SetButtonClicked(TextGraphicHigh, false);
		SetButtonClicked(TextGraphicMax, true);
		SetButtonClicked(TextGraphicCustom, false);
	}
}

//------------------------------------------------------------
void USettingWidget::OnMasterVolumeChanged(float ChangedValue)
{
	if (_SettingManager)
	{
		ATestSettingActor* SettingManager = Cast<ATestSettingActor>(_SettingManager);
		MasterVolumeValue = UKismetMathLibrary::FClamp(ChangedValue, 0.0f, 1.0f);
		SettingManager->SetMasterVolume(MasterVolumeValue);
	}
}

void USettingWidget::OnEffectVolumeChanged(float ChangedValue)
{
	if (_SettingManager)
	{
		ATestSettingActor* SettingManager = Cast<ATestSettingActor>(_SettingManager);
		EffectVolumeValue = UKismetMathLibrary::FClamp(ChangedValue, 0.0f, 1.0f);
		SettingManager->SetEffectVolume(EffectVolumeValue);
	}
}

void USettingWidget::OnMouseSensitivityChanged(float ChangedValue)
{
	if (_SettingManager)
	{
		ATestSettingActor* SettingManager = Cast<ATestSettingActor>(_SettingManager);
		MouseSensitivityValue = UKismetMathLibrary::FClamp(ChangedValue, 0.0f, 1.0f);
		SettingManager->SetMouseSensitivity(MouseSensitivityValue);
	}
}


//-------------------------------------------------------------------------------------
void USettingWidget::SetButtonClicked(UTextBlock* Text, bool b)
{
	if (b)
	{
		FSlateFontInfo FI = Text->Font;
		FI.Size = 30;
		Text->SetFont(FI);

		FSlateColor SC = FSlateColor(FColor::White);
		Text->SetColorAndOpacity(SC);
	}
	else
	{
		FSlateFontInfo FI = Text->Font;
		FI.Size = 24;
		Text->SetFont(FI);

		FSlateColor SC = FSlateColor(FColor(139, 139, 139));
		Text->SetColorAndOpacity(SC);
	}
}

void USettingWidget::HighlightButtons()
{
	if (_SettingManager)
	{
		ATestSettingActor* SettingManager = Cast<ATestSettingActor>(_SettingManager);

		switch (SettingManager->GetResolutionQuality())
		{
		case 1:
			SetButtonClicked(TextResolution1280, true);
			break;
		case 2:
			SetButtonClicked(TextResolution1336, true);
			break;
		case 3:
			SetButtonClicked(TextResolution1920, true);
			break;
		case 4:
			SetButtonClicked(TextResolution2560, true);
			break;
		case 5:
			SetButtonClicked(TextResolution3840, true);
			break;
		default:
			break;
		}

		//VSync
		if(SettingManager->GetVSyncEnabled())
			SetButtonClicked(TextVSyncOn, true);
		else
			SetButtonClicked(TextVSyncOff, true);

		//Graphic
		switch (SettingManager->GetGraphicQuality())
		{
		case 0:
			SetButtonClicked(TextGraphicCustom, true);
			break;
		case 1:
			SetButtonClicked(TextGraphicLow, true);
			break;
		case 2:
			SetButtonClicked(TextGraphicMid, true);
			break;
		case 3:
			SetButtonClicked(TextGraphicHigh, true);
			break;
		case 4:
			SetButtonClicked(TextGraphicMax, true);
			break;
		default:
			break;
		}
	}
}

void USettingWidget::OnHovered()
{
	FSlateFontInfo FI = BackButtonText->Font;
	FI.Size = 37;
	BackButtonText->SetFont(FI);

	FSlateColor SC = FSlateColor(FColor::White);
	BackButtonText->SetColorAndOpacity(SC);
}

void USettingWidget::OnUnHovered()
{
	FSlateFontInfo FI = BackButtonText->Font;
	FI.Size = 31;
	BackButtonText->SetFont(FI);

	FSlateColor SC = FSlateColor(FColor(139, 139, 139));
	BackButtonText->SetColorAndOpacity(SC);
}

void USettingWidget::BackToESCMenuWidget()
{
	RemoveFromParent();
	if (_ESCMenuWidget)
	{
		ESCMenuWidget = Cast<UESCMenuWidget>(CreateWidget(GetWorld(), _ESCMenuWidget));
		if (ESCMenuWidget)
		{
			ESCMenuWidget->AddToViewport();
		}
	}
}
