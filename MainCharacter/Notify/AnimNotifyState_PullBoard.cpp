// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_PullBoard.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

UAnimNotifyState_PullBoard::UAnimNotifyState_PullBoard()
{

}

void UAnimNotifyState_PullBoard::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{

	if (MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
	{
		MainCharacter = Cast<AMainCharacter>(MeshComp->GetOwner());
		if (MainCharacter != nullptr)
		{

			UCapsuleComponent* TriggerCapsuleComponent = MainCharacter->TriggerCapsuleComponent;
			FVector CharacterForwardVector = TriggerCapsuleComponent->GetForwardVector();

			FLatentActionInfo Info;
			Info.CallbackTarget = this;
			UKismetSystemLibrary::MoveComponentTo
			(
				TriggerCapsuleComponent,
				TriggerCapsuleComponent->GetRelativeLocation() + CharacterForwardVector * -30.0f,
				TriggerCapsuleComponent->GetRelativeRotation(),
				true,
				true,
				0.5,
				false,
				EMoveComponentAction::Move,
				Info
			);
		}
	}
}