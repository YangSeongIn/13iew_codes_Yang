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
}

void UEndingComputerScreen3DWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	
}

FString UEndingComputerScreen3DWidget::SetTimeToString(float Time)
{
	float h = Time / 3600.0f;
	float m = fmod(Time, 3600.0f) / 60.0f;
	float s = fmod(Time, 60.0f);
	FString TimeFormat = FString::Printf(TEXT("%02f:%02f:%02f"), h, m, s);
	return TimeFormat;
}