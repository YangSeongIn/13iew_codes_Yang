#include "SlotDetailWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "MyGameInstance.h"
#include "SaveGameObject.h"
#include "Kismet/GameplayStatics.h"
#include "SlotDeleteCheckWidget.h"

void USlotDetailWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FString SlotNameTextSetting;
	SlotNameTextSetting = FString::Printf(TEXT("Slot%d"), SlotIndex);
	DetailText->SetText(FText::FromString(SlotNameTextSetting));

	StartButton->OnClicked.AddDynamic(this, &USlotDetailWidget::StartGame);
	DeleteButton->OnClicked.AddDynamic(this, &USlotDetailWidget::DeleteGame);
	CloseButton->OnClicked.AddDynamic(this, &USlotDetailWidget::CloseWidget);
}

void USlotDetailWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	if (DeleteButton->GetIsEnabled())
	{
		DeleteButton->bIsEnabled = true;
	}
	else
	{
		DeleteButton->bIsEnabled = false;
	}
}

void USlotDetailWidget::StartGame()
{
	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->SaveSlotName = SlotName;
		GameInstance->SlotDetailWidget = this;
		GameInstance->LoadGame();	
		UGameplayStatics::OpenLevel(GetWorld(), FName(GameInstance->SaveGameData->SaveLevelName));
	}
}

void USlotDetailWidget::DeleteGame()
{
	if (_DeleteCheckWidget)
	{
		DeleteCheckWidget = Cast<USlotDeleteCheckWidget>(CreateWidget(GetWorld(), _DeleteCheckWidget));
		if (DeleteCheckWidget)
		{
			DeleteCheckWidget->SlotName = SlotName;
			DeleteCheckWidget->DetailWidget = this;
			DeleteCheckWidget->AddToViewport();
		}
	}
}

void USlotDetailWidget::CloseWidget()
{
	RemoveFromParent();
}

void USlotDetailWidget::SetDeleteButtonEnabled()
{
	DeleteButton->bIsEnabled = false;
}

void USlotDetailWidget::SetTexture() {
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, FString::Printf(TEXT("test-%s"), *SavedPoint));

	SlotLevel->SetText(FText::FromString(SavedPoint));

	if (SavedPoint.Equals("Empty"))
	{
		FString Path = FString("/Game/MJs/Imgs/UI/DetailWidget_Empty");
		UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *Path));

		BackGroundImage->SetBrushFromTexture(Texture);
	}
	else if(SavedPoint.Equals("Basement"))	//level1
	{
		FString Path = FString("/Game/MJs/Imgs/UI/DetailWidget_Basement");
		UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *Path));

		BackGroundImage->SetBrushFromTexture(Texture);
	}
	else if(SavedPoint.Equals("Hospital"))	//level2
	{
		FString Path = FString("/Game/MJs/Imgs/UI/DetailWidget_Hospital");
		UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *Path));

		BackGroundImage->SetBrushFromTexture(Texture);
	}
}