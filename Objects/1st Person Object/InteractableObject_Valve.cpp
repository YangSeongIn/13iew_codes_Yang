// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObject_Valve.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "InteractableObject_Valve.h"
#include "Kismet/KismetMathLibrary.h"
#include "GaugeWidget.h"
#include "HighlightableComponent.h"
#include "SoundManager.h"
#include "Components/AudioComponent.h"

AInteractableObject_Valve::AInteractableObject_Valve()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
}

void AInteractableObject_Valve::BeginPlay()
{
	Super::BeginPlay();

	MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void AInteractableObject_Valve::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GaugeWidget)
	{
		if (bIsInteracting)
		{
			if (!UKismetMathLibrary::NearlyEqual_FloatFloat(ValveGauge, 100.0f, 0.1) && MainCharacter && MainCharacter->bIsOnFocusHighlightable)
			{
				ProgressingGauge();
				LinkedActorAction(ValveGauge);
			}
			else if(!MainCharacter->bIsOnFocusHighlightable)
			{
				if (!bIsFinished)
				{
					ReverseGauge();
					if (GaugeWidget->IsInViewport())
					{
						GaugeWidget->RemoveFromParent();
					}
				}	
			}
		}
		else
		{
			if (!bIsFinished)
			{
				ReverseGauge();
			}
		}			
	}
}

void AInteractableObject_Valve::Interact()
{
	if (bCanInteract)
	{
		bCanInteract = false;
		FTimerHandle WaitHandle;
		float WaitTime = 0.5f;
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
			{
				bCanInteract = true;
			}), WaitTime, false);
		SetActorTickEnabled(true);
		if (SoundManager && bPlayMusic)
		{
			HorrorAudio = SoundManager->PlayValveGaugeHorrorSound();
			HorrorAudio->FadeIn(10, 1, 0, EAudioFaderCurve::SCurve);
		}
		if (_GaugeWidget)
		{
			GaugeWidget = Cast<UGaugeWidget>(CreateWidget(GetWorld(), _GaugeWidget));
			if (GaugeWidget)
			{
				GaugeWidget->AddToViewport();
				bIsInteracting = true;
			}
		}
	}
}

void AInteractableObject_Valve::StopInteract()
{
	MainCharacter->bIsActivatingValve = false;
	bIsInteracting = false;

	//GaugeWidget->SetValveGauge(0, false);
	if (GaugeWidget->IsInViewport())
	{
		GaugeWidget->RemoveFromParent();
	}

	if (HorrorAudio && !bIsFinished)
	{
		HorrorAudio->DestroyComponent();
	}
}

void AInteractableObject_Valve::LinkedActorAction(float Gauge)
{
	if (LinkedActor)
	{
		LinkedActor->AddActorWorldOffset(FVector(0, 0, LinkedActorSpeed * abs(sin(Gauge / 3 / ValveRotationCycleTime))));
	}
}


void AInteractableObject_Valve::ProgressingGauge()
{
	if (UKismetMathLibrary::NearlyEqual_FloatFloat(ValveGauge, 100.0f, 0.5f))
	{
		ValveGaugeComplete();
	}
	else
	{
		MainCharacter->bIsActivatingValve = true;
		ValveGauge += ValveGaugeSpeed;
		ValveGauge = FMath::Clamp(ValveGauge, 0.0f, 100.0f);
		GaugeWidget->SetValveGauge(ValveGauge, true);

		float ValveRotationYawValue = abs(sin((ValveGauge / 3) * (1 / ValveRotationCycleTime))) * ValveRotationSpeed;
		StaticMesh->AddRelativeRotation(FRotator(0, ValveRotationYawValue, 0));
		ValveRotateValue += ValveRotationYawValue;
		LinkedActorAction(ValveGauge);
	}
}

void AInteractableObject_Valve::ReverseGauge()
{
	if (HorrorAudio)
	{
		HorrorAudio->DestroyComponent();
	}
	MainCharacter->bIsActivatingValve = false;
	if (!UKismetMathLibrary::NearlyEqual_FloatFloat(ValveRotateValue, 0.0f, 0.1) && ValveRotateValue > 0.0f)
	{
		if (ValveRotateValue - ValveReverseSpeed >= 0)
		{
			ReverseGaugeEvent(1);
		}
		else
		{
			ReverseGaugeEvent(2);
		}
		bCanInteract = false;
	}
	else
	{
		SetActorTickEnabled(false);
		bCanInteract = true;
	}
}

void AInteractableObject_Valve::ReverseGaugeEvent(float FSpeed)
{
	ValveRotateValue -= ValveReverseSpeed / FSpeed;
	ValveGauge -= ValveReverseSpeed / FSpeed;
	StaticMesh->AddRelativeRotation(FRotator(0, -ValveReverseSpeed / FSpeed, 0));
	if (LinkedActor)
	{
		LinkedActor->AddActorWorldOffset(FVector(0, 0, -7 * LinkedActorSpeed / FSpeed));
	}
}

void AInteractableObject_Valve::ValveGaugeComplete()
{
	bCanInteract = false;
	HightlightableComponent->DestroyComponent();
	StaticMesh->SetRenderCustomDepth(false);
	bIsFinished = true;
	MainCharacter->bIsActivatingValve = false;
	if (GaugeWidget->IsInViewport())
	{
		GaugeWidget->RemoveFromParent();
	}
	if (HorrorAudio)
	{
		HorrorAudio->FadeOut(2, 0);
	}
}