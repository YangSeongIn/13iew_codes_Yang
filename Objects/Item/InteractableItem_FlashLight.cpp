#include "InteractableItem_FlashLight.h"
#include "MainCharacter.h"
#include "KeyGuideWidget.h"
#include "MyGameInstance.h"
#include "SaveGameObject.h"

AInteractableItem_FlashLight::AInteractableItem_FlashLight()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
}

void AInteractableItem_FlashLight::BeginPlay()
{
	Super::BeginPlay();

	MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->OnReset.AddUFunction(this, FName("Reset"));
		GameInstance->OnSave.AddUFunction(this, FName("Save"));
	}
}

void AInteractableItem_FlashLight::Interact()
{
	MainCharacter->bHaveFlashLight = true;
	bIsDestroyed = true;
	StaticMesh->SetVisibility(false);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (_KeyGuideWidget)
	{
		KeyGuideWidget = Cast<UKeyGuideWidget>(CreateWidget(GetWorld(), _KeyGuideWidget));
		if (KeyGuideWidget)
		{
			KeyGuideWidget->SetKeyGuideText("FlashLight");
			KeyGuideWidget->AddToViewport();
		}

		FTimerHandle WaitHandle;
		float WaitTime = 5.0f;
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
			{
				KeyGuideWidget->RemoveFromParent();
			}), WaitTime, false);
	}
}

void AInteractableItem_FlashLight::Reset()
{
	bIsDestroyed = GameInstance->SaveGameData->Level1SaveDataStruct.bFlashLightIsDestroyed;

	if (bIsDestroyed)
	{
		StaticMesh->SetVisibility(false);
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}


void AInteractableItem_FlashLight::Save()
{
	GameInstance->SaveGameData->Level1SaveDataStruct.bFlashLightIsDestroyed = bIsDestroyed;
}