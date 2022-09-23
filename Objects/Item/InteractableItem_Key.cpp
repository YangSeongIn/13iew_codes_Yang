#include "InteractableItem_Key.h"
#include "MainCharacter.h"
#include "KeyInventoryWidget.h"

AInteractableItem_Key::AInteractableItem_Key()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
}

void AInteractableItem_Key::BeginPlay()
{
	Super::BeginPlay();

	MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void AInteractableItem_Key::Interact()
{
	GetKey();
	Destroy();
}

void AInteractableItem_Key::GetKey()
{
	if (MainCharacter)
	{
		MainCharacter->Inventory.Add(this);
		OnKeyInventoryWidget();
	}
}

void AInteractableItem_Key::OnKeyInventoryWidget()
{
	if (KeyInventoryWidgetClass)
	{
		KeyInventoryWidget = Cast<UKeyInventoryWidget>(CreateWidget(GetWorld(), KeyInventoryWidgetClass));
		if (KeyInventoryWidget)
		{
			KeyInventoryWidget->AddToViewport();
		}
	}
}

void AInteractableItem_Key::OffKeyInventoryWidget()
{
	if (KeyInventoryWidget->IsInViewport())
	{
			KeyInventoryWidget->RemoveFromParent();
	}
}