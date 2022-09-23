#include "AnimNotifyState_FootStep.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

UAnimNotifyState_FootStep::UAnimNotifyState_FootStep()
{
	static ConstructorHelpers::FObjectFinder<USoundCue> ConcreteSoundCueAsset(TEXT("SoundCue'/Game/Sounds/FootStep/Sounds/WaveFiles/Footsteps_Mono/concrete_1_Cue.concrete_1_Cue'"));
	if (ConcreteSoundCueAsset.Succeeded())
	{
		ConcreteSoundCue = ConcreteSoundCueAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> CarpetSoundCueAsset(TEXT("SoundCue'/Game/Sounds/FootStep/Sounds/WaveFiles/Footsteps_Mono/carpet_heels_6_Cue.carpet_heels_6_Cue'"));
	if (CarpetSoundCueAsset.Succeeded())
	{
		CarpetSoundCue = CarpetSoundCueAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> GrassSoundCueAsset(TEXT("SoundCue'/Game/Sounds/FootStep/Sounds/WaveFiles/Footsteps_Mono/Grass_Heels_1_Cue.Grass_Heels_1_Cue'"));
	if (GrassSoundCueAsset.Succeeded())
	{
		GrassSoundCue = GrassSoundCueAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> GravelSoundCueAsset(TEXT("SoundCue'/Game/Sounds/FootStep/Sounds/WaveFiles/Footsteps_Mono/Gravel_Heels_1_Cue.Gravel_Heels_1_Cue'"));
	if (GravelSoundCueAsset.Succeeded())
	{
		GravelSoundCue = GravelSoundCueAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> GroundSoundCueAsset(TEXT("SoundCue'/Game/Sounds/FootStep/Sounds/WaveFiles/Footsteps_Mono/Ground_Heels_6_Cue.Ground_Heels_6_Cue'"));
	if (GroundSoundCueAsset.Succeeded())
	{
		GroundSoundCue = GroundSoundCueAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> MetalSoundCueAsset(TEXT("SoundCue'/Game/Sounds/FootStep/Sounds/WaveFiles/Footsteps_Mono/Metal_Heels_1_Cue.Metal_Heels_1_Cue'"));
	if (MetalSoundCueAsset.Succeeded())
	{
		MetalSoundCue = MetalSoundCueAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> PlasticSoundCueAsset(TEXT("SoundCue'/Game/Sounds/FootStep/Sounds/WaveFiles/Footsteps_Mono/Plastic_1_Cue.Plastic_1_Cue'"));
	if (PlasticSoundCueAsset.Succeeded())
	{
		PlasticSoundCue = PlasticSoundCueAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> SandSoundCueAsset(TEXT("SoundCue'/Game/Sounds/FootStep/Sounds/WaveFiles/Footsteps_Mono/Sand_Heels_1_Cue.Sand_Heels_1_Cue'"));
	if (SandSoundCueAsset.Succeeded())
	{
		SandSoundCue = SandSoundCueAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> WaterSoundCueAsset(TEXT("SoundCue'/Game/Sounds/FootStep/Sounds/WaveFiles/Footsteps_Mono/water_1_Cue.water_1_Cue'"));
	if (WaterSoundCueAsset.Succeeded())
	{
		WaterSoundCue = WaterSoundCueAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> WoodSoundCueAsset(TEXT("SoundCue'/Game/Sounds/FootStep/Sounds/WaveFiles/Footsteps_Mono/wood_1_Cue.wood_1_Cue'"));
	if (WoodSoundCueAsset.Succeeded())
	{
		WoodSoundCue = WoodSoundCueAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> SnowSoundCueAsset(TEXT("SoundCue'/Game/Sounds/FootStep/Sounds/WaveFiles/Footsteps_Mono/Snow_Heels_1_Cue.Snow_Heels_1_Cue'"));
	if (SnowSoundCueAsset.Succeeded())
	{
		SnowSoundCue = SnowSoundCueAsset.Object;
	}
}

void UAnimNotifyState_FootStep::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
	{
		MainCharacter = Cast<AMainCharacter>(MeshComp->GetOwner());
		if (MainCharacter != nullptr)
		{
			FVector StartLoc = MainCharacter->TriggerCapsuleComponent->GetRelativeLocation();
			
			FVector EndLoc = StartLoc - FVector(0.0f, 0.0f, 100.0f);
			TArray<AActor*> ToIgnore;
			FHitResult OutHit;
			bool bIsOutHit = UKismetSystemLibrary::LineTraceSingle(MainCharacter->GetWorld(), StartLoc, EndLoc, ETraceTypeQuery::TraceTypeQuery1, false,
				ToIgnore, EDrawDebugTrace::None, OutHit, true);

			if (bIsOutHit)
			{
				//EPhysicalSurface::SurfaceType1
				switch (UGameplayStatics::GetSurfaceType(OutHit))
				{
				case SurfaceType1:
					MainCharacter->MakeNoise(CarpetSoundCue, OutHit.Location); 
					break;
				case SurfaceType2:
					MainCharacter->MakeNoise(ConcreteSoundCue, OutHit.Location);
					break;
				case SurfaceType3:
					MainCharacter->MakeNoise(GrassSoundCue, OutHit.Location);
					break;
				case SurfaceType4:
					MainCharacter->MakeNoise(GravelSoundCue, OutHit.Location);
					break;
				case SurfaceType5:
					MainCharacter->MakeNoise(GroundSoundCue, OutHit.Location);
					break;
				case SurfaceType6:
					MainCharacter->MakeNoise(MetalSoundCue, OutHit.Location);
					break;
				case SurfaceType7:
					MainCharacter->MakeNoise(PlasticSoundCue, OutHit.Location);
					break;
				case SurfaceType8:
					MainCharacter->MakeNoise(SandSoundCue, OutHit.Location);
					break;
				case SurfaceType9:
					MainCharacter->MakeNoise(SnowSoundCue, OutHit.Location);
					break;
				case SurfaceType10:
					MainCharacter->MakeNoise(WaterSoundCue, OutHit.Location);
					break;
				case SurfaceType11:
					MainCharacter->MakeNoise(WoodSoundCue, OutHit.Location);
					break;
				default:
					break;
				}
			}
		}
	}
}