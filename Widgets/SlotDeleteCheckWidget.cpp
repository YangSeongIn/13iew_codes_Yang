#include "SlotDeleteCheckWidget.h"
#include "Components/Button.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SlotDetailWidget.h"

void USlotDeleteCheckWidget::NativeConstruct()
{
	Super::NativeConstruct();

	DeleteButton->OnClicked.AddDynamic(this, &USlotDeleteCheckWidget::DeleteGame);
	CloseButton->OnClicked.AddDynamic(this, &USlotDeleteCheckWidget::CloseWidget);
}

void USlotDeleteCheckWidget::DeleteGame()
{
	UGameplayStatics::DeleteGameInSlot(SlotName, 0);
	DetailWidget->DeleteButton->bIsEnabled = false;
	RemoveFromParent();
	DetailWidget->RemoveFromParent();
}

void USlotDeleteCheckWidget::CloseWidget()
{
	RemoveFromParent();
}