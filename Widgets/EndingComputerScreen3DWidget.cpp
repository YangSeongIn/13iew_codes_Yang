#include "EndingComputerScreen3DWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "MyGameInstance.h"
#include "SaveGameObject.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UEndingComputerScreen3DWidget::NativeConstruct()
{
	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (GameInstance)
	{
		FString s11 = SetTimeToString(GameInstance->SaveGameData->TotalPlayTime);
		Time_11th->SetText(FText::FromString(s11));

		FString s12 = SetTimeToString(GameInstance->SaveGameData->TotalPlayTime + 64);
		Time_12th->SetText(FText::FromString(s12));

		FString s13 = SetTimeToString(GameInstance->SaveGameData->TotalPlayTime + 124);
		Time_13th->SetText(FText::FromString(s13));
	}
}

void UEndingComputerScreen3DWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	
}

FString UEndingComputerScreen3DWidget::SetTimeToString(float Time)
{
	int h = (int)(Time / 3600.0f);
	int m = (int)(fmod(Time, 3600.0f) / 60.0f);
	int s = (int)(fmod(Time, 60.0f));
	FString TimeFormat = FString::Printf(TEXT("%02d:%02d:%02d"), h, m, s);
	return TimeFormat;
}