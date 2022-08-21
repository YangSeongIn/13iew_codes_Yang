// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_Ladder.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

UAnimNotifyState_Ladder::UAnimNotifyState_Ladder()
{
	static ConstructorHelpers::FObjectFinder<USoundCue> LadderSoundCueAsset(TEXT("SoundCue'/Game/Sounds/Ladder/Cue_Ladder.Cue_Ladder'"));
	if (LadderSoundCueAsset.Succeeded())
	{
		LadderSoundCue = LadderSoundCueAsset.Object;
	}
}

void UAnimNotifyState_Ladder::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
	{
		MainCharacter = Cast<AMainCharacter>(MeshComp->GetOwner());
		if (MainCharacter != nullptr)
		{
			FVector SoundLocation = MainCharacter->SkeletalMesh->GetSocketLocation(FName("hand_r")) + MainCharacter->SkeletalMesh->GetSocketLocation(FName("hand_l")) / 2;
			MainCharacter->MakeNoise(LadderSoundCue, SoundLocation);
		}
	}
}
