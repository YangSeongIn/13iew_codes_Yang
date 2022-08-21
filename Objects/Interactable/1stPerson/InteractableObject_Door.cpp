// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObject_Door.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "MyGameInstance.h"
#include "MainCharacter.h"
#include "SaveGameObject.h"

#include "Waypoints.h"
#include "AI_Bot_Character.h"
#include "AIController.h"
#include "AI_Bot_Controller.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

AInteractableObject_Door::AInteractableObject_Door()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(DefaultSceneRoot);

	DoorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorTimelineFront"));

	InterpFunction.BindUFunction(this, FName("InterpReturn"));
	TimelineFinish.BindUFunction(this, FName("OnFinish"));
}

void AInteractableObject_Door::OpenForAI()
{

}

void AInteractableObject_Door::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->OnReset.AddUFunction(this, FName("Reset"));
		GameInstance->OnSave.AddUFunction(this, FName("Save"));
	}

	if (DoorCurveFloat && DoorTimeline)
	{
		DoorTimeline->AddInterpFloat(DoorCurveFloat, InterpFunction, FName("Door"));
		DoorTimeline->SetTimelineFinishedFunc(TimelineFinish);
		DoorTimeline->SetLooping(false);
	}

	//BoxComponent = FindComponentByClass<UBoxComponent>();
	//BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AInteractableObject_Door::OnOverlapBegin);
}

void AInteractableObject_Door::Interact()
{
	InteractCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	if (InteractCharacter)
	{
		if ((bUseKey && Key && InteractCharacter->Inventory.Contains(Key)) || !bUseKey)
		{
			float FrontBack = FMath::Sign(FVector::DotProduct(GetActorForwardVector(), InteractCharacter->FirstPersonCameraComponent->GetForwardVector()));

			FrontBack < 0 ? bIsFront = true : bIsFront = false;
			if (bIsClosed)
			{
				if (bIsFrontDoor)
				{
					bIsFront ? f = -1.0f : f = 1.0f;
				}
				else
				{
					bIsFront ? f = 1.0f : f = -1.0f;
				}
				DoorTimeline->Play();
			}
			else
			{
				DoorTimeline->Reverse();
			}
			bIsClosed = !bIsClosed;
		}
	}
}

void AInteractableObject_Door::OnFocus()
{
	LAI.CallbackTarget = this;
	LAI.ExecutionFunction = "DelayFunc";
	LAI.Linkage = 0;
	UKismetSystemLibrary::RetriggerableDelay(GetWorld(), 0.1f, LAI);

	while (DoN < 1)
	{
		DoN++;
		FirstPersonDoorWidget = Cast<UFirstPersonDefaultWidget>(CreateWidget(GetWorld(), DoorWidgetClass));
		if (FirstPersonDoorWidget)
		{
			FirstPersonDoorWidget->AddToViewport();
		}
	}
}

void AInteractableObject_Door::DelayFunc()
{
	if (FirstPersonDoorWidget && FirstPersonDoorWidget->IsInViewport())
	{
		FirstPersonDoorWidget->RemoveFromParent();
		DoN = 0;	// Reset
	}

}

void AInteractableObject_Door::OnFinish()
{

}

void AInteractableObject_Door::InterpReturn(float value)
{
	StaticMesh->SetRelativeRotation(FRotator(0, UKismetMathLibrary::Ease(0, f * 110, value, EEasingFunc::SinusoidalInOut), 0));
}

void AInteractableObject_Door::Reset()
{
	if (GameInstance->SaveGameData->Level1SaveDataStruct.DoorData.Contains(this))
	{
		bIsClosed = *GameInstance->SaveGameData->Level1SaveDataStruct.DoorData.Find(this);
		if (!bIsClosed)
		{
			StaticMesh->SetRelativeRotation(FRotator(0, -110, 0));
		}
	}
}

void AInteractableObject_Door::Save()
{
	GameInstance->SaveGameData->Level1SaveDataStruct.DoorData.Add(this, bIsClosed);
}


void AInteractableObject_Door::OnOverlapBegin(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AAI_Bot_Character* AI = Cast<AAI_Bot_Character>(OtherActor);
	if (AI)
	{
		float AIFrontBack = FMath::Sign(FVector::DotProduct(GetActorForwardVector(), AI->GetActorForwardVector()));

		AIFrontBack < 0 ? bIsFront = true : bIsFront = false;
		if (bIsClosed)
		{
			if (bIsFrontDoor)
			{
				bIsFront ? f = -1.0f : f = 1.0f;
			}
			else
			{
				bIsFront ? f = 1.0f : f = -1.0f;
			}
			DoorTimeline->Play();
			UE_LOG(LogTemp, Log, TEXT("!!"));
		}
		else
		{
			DoorTimeline->Reverse();
		}
		bIsClosed = !bIsClosed;
	}
}

	