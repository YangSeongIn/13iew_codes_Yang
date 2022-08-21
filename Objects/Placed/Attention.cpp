// Fill out your copyright notice in the Description page of Project Settings.


#include "Attention.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MainCharacter.h"
#include "MainCharacterAnimInstance.h"
#include "Interface_Attention.h"

AAttention::AAttention()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AAttention::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector StartLoc = GetActorLocation();
	FVector EndLoc = GetActorLocation();
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);

	FHitResult HitResult;

	bool hit = UKismetSystemLibrary::SphereTraceSingle
	(
		GetWorld(),
		StartLoc,
		EndLoc,
		SphereSize,
		TraceTypeQuery9,
		false,
		IgnoreActors,
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true
	);

	if (hit)
	{
		if (MainCharacter)
		{
			UMainCharacterAnimInstance* AnimInstance = Cast<UMainCharacterAnimInstance>(MainCharacter->SkeletalMesh->GetAnimInstance());
			if (AnimInstance)
			{
				IInterface_Attention* Interface_Attention = Cast<IInterface_Attention>(AnimInstance);
				if (Interface_Attention)
				{
					float DotValue = UKismetMathLibrary::Dot_VectorVector(UKismetMathLibrary::Normal(GetActorLocation() - MainCharacter->GetActorLocation()), MainCharacter->GetActorForwardVector());
					if (DotValue > -0.4f)
					{
						Interface_Attention->AttentionOn(this);
					}
					else
					{
						Interface_Attention->AttentionOff();
					}
				}
			}
		}
		else
		{
			MainCharacter = Cast<AMainCharacter>(HitResult.Actor);
		}
	}
}

