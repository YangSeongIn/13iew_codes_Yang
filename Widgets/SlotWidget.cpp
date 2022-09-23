#include "SlotWidget.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "SlotDetailWidget.h"
#include "SaveGameObject.h"
#include "MainMenuWidget.h"
#include "MultiSlotWidget.h"
#include "SettingWidget.h"
#include "CreditWidget.h"

void USlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	SlotButton->OnClicked.AddDynamic(this, &USlotWidget::StartSlot);
	SlotButton->OnHovered.AddDynamic(this, &USlotWidget::OnHovered);
	SlotButton->OnUnhovered.AddDynamic(this, &USlotWidget::OnUnHovered);


}

void USlotWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);
	SetSlotInfoText();
}

void USlotWidget::SetSlotInfoText()
{
	if (SlotIndex > 0)
	{
		FString SlotNum;
		SlotNum = FString::Printf(TEXT("%d."), SlotIndex);

		FString SlotInfo;
		bool bDoesSaveGameExist = UGameplayStatics::DoesSaveGameExist(SlotName, 0);
		SaveGameData = Cast<USaveGameObject>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));


		if (bDoesSaveGameExist)
		{
			SavedPoint = SaveGameData->SavePointName;
			SlotInfo = SavedPoint + " - " +  FormatTimeToHHMMSS(SaveGameData->TotalPlayTime);
		}
		else
		{
			SavedPoint = FString("Empty");
			SlotInfo = FString::Printf(TEXT("Empty"));
		}
		SlotNameText->SetText(FText::FromString(SlotNum + SlotInfo));
	}
	else
	{
		SlotNameText->SetText(FText::FromString(SlotName));
	}
}

void USlotWidget::StartSlot()
{
	if (SlotIndex > 0)
	{
		if (_DetailWidget)
		{
			DetailWidget = Cast<USlotDetailWidget>(CreateWidget(GetWorld(), _DetailWidget));
			if (DetailWidget)
			{
				DetailWidget->SlotIndex = SlotIndex;
				DetailWidget->SlotName = SlotName;
				DetailWidget->SavedPoint = SavedPoint;

				if(SavedPoint == "Empty")
					DetailWidget->PlayedTime->SetText(FText::FromString(""));
				else
					DetailWidget->PlayedTime->SetText(FText::FromString(FormatTimeToHHMMSS(SaveGameData->TotalPlayTime)));

				UE_LOG(LogTemp, Log, TEXT("%s"), *SlotName); 
				bool bIsSlotNull = !UGameplayStatics::DoesSaveGameExist(SlotName, 0);
				bIsSlotNull ? DetailWidget->DeleteButton->bIsEnabled = false : DetailWidget->DeleteButton->bIsEnabled = true;

				DetailWidget->AddToViewport();
				DetailWidget->SetTexture();
			}
		}

	}
	else
	{
		//-----------
		// main menu
		if (SlotName.Equals("Play"))
		{
			//RemoveFromParent();
			if (_MultiSlotWidget)
			{
				MultiSlotWidget = Cast<UMultiSlotWidget>(CreateWidget(GetWorld(), _MultiSlotWidget));
				if (MultiSlotWidget)
				{
					if (OnClickStartGame.IsBound() == true)
					{
						OnClickStartGame.Execute();
					}
					MultiSlotWidget->AddToViewport();
				}
			}
		}
		else if (SlotName.Equals("Setting"))
		{
			if (_SettingWidget)
			{
				UE_LOG(LogTemp, Log, TEXT("SlotWidget::StartSlot::Set Option True"));

				SettingWidget = Cast<USettingWidget>(CreateWidget(GetWorld(), _SettingWidget));
				if (SettingWidget)
				{
					SettingWidget->AddToViewport();
					SettingWidget->ParentType = 0;
				}
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("SlotWidget::StartSlot::Set Option False"));
			}
		}
		else if (SlotName.Equals("Credit"))
		{
			if (_CreditWidget)
			{
				UE_LOG(LogTemp, Log, TEXT("SlotWidget::StartSlot::Credit"));

				CreditWidget = Cast<UCreditWidget>(CreateWidget(GetWorld(), _CreditWidget));
				if (CreditWidget)
				{
					CreditWidget->AddToViewport();
				}
			}
		}
		else if (SlotName.Equals("Quit"))              
		{
			UKismetSystemLibrary::QuitGame(this, 0, EQuitPreference::Quit, false);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("SlotWidget::StartSlot::Else"));
		}
	}
}

void USlotWidget::OnHovered()
{
	FSlateFontInfo FI = SlotNameText->Font;
	FI.Size = 37;
	SlotNameText->SetFont(FI);

	FSlateColor SC = FSlateColor(FColor::White);
	SlotNameText->SetColorAndOpacity(SC);
}

void USlotWidget::OnUnHovered() 
{
	FSlateFontInfo FI = SlotNameText->Font;
	FI.Size = 31;
	SlotNameText->SetFont(FI);

	FSlateColor SC = FSlateColor(FColor(139, 139, 139));
	SlotNameText->SetColorAndOpacity(SC);
}

FString USlotWidget::FormatTimeToHHMMSS(float t) 
{
	FString str = "";

	if (t >= 3600)
	{
		int h = (int)(t / 3600);
		t -= h * 3600;
		str = str + FString::Printf(TEXT("%d:"), h);
	}
	else
	{
		str = str + "00:";
	}

	if (t >= 60)
	{
		int m = (int)(t / 60);
		t -= m * 60;
		str = str + FString::Printf(TEXT("%02d:"), m);
	}
	else
	{
		str = str + "00:";
	}

	int s = (int)t;
	str = str + FString::Printf(TEXT("%02d"), s);


	return str;
}