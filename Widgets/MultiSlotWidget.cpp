#include "MultiSlotWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "MyGameInstance.h"
#include "SlotDetailWidget.h"
#include "MainMenuWidget.h"
#include "SlotWidget.h"

void UMultiSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Widget_Slot_1->SlotIndex = 1;
	Widget_Slot_2->SlotIndex = 2;
	Widget_Slot_3->SlotIndex = 3;
	Widget_Slot_1->SlotName = FString("Slot1");
	Widget_Slot_2->SlotName = FString("Slot2");
	Widget_Slot_3->SlotName = FString("Slot3");

	BackButton->OnHovered.AddDynamic(this, &UMultiSlotWidget::OnHovered);
	BackButton->OnUnhovered.AddDynamic(this, &UMultiSlotWidget::OnUnHovered);
	BackButton->OnClicked.AddDynamic(this, &UMultiSlotWidget::BackToMainMenu);
}



void UMultiSlotWidget::OnHovered()
{
	FSlateFontInfo FI = BackButtonText->Font;
	FI.Size = 37;
	BackButtonText->SetFont(FI);

	FSlateColor SC = FSlateColor(FColor::White);
	BackButtonText->SetColorAndOpacity(SC);
}

void UMultiSlotWidget::OnUnHovered()
{
	FSlateFontInfo FI = BackButtonText->Font;
	FI.Size = 31;
	BackButtonText->SetFont(FI);

	FSlateColor SC = FSlateColor(FColor(139, 139, 139));
	BackButtonText->SetColorAndOpacity(SC);
}

void UMultiSlotWidget::BackToMainMenu()
{
	RemoveFromParent();
	if (_MainMenuWidget)
	{
		MainMenuWidget = Cast<UMainMenuWidget>(CreateWidget(GetWorld(), _MainMenuWidget));
		if (MainMenuWidget)
		{
			MainMenuWidget->AddToViewport();
		}
	}
}