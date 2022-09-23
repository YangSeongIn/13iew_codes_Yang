#include "Projector.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "WayPoints.h"
#include "AI_Bot_Character.h"

AProjector::AProjector()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Comp"));
	BoxComponent->SetupAttachment(RootComponent);

	SpotLight_B = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight_B"));
	SpotLight_B->SetupAttachment(RootComponent);
	

	SpotLight_W = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight_W"));
	SpotLight_W->SetupAttachment(RootComponent);
	
}

void AProjector::BeginPlay()
{
	Super::BeginPlay();

	SpotLight_B->SetVisibility(false);
	SpotLight_W->SetVisibility(false);

	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->OnReset.AddUFunction(this, FName("Reset"));
	}
	SM_Screen = Cast<UStaticMeshComponent>(Screen->FindComponentByClass(UStaticMeshComponent::StaticClass()));
	DefaultMaterial = SM_Screen->GetMaterial(0);
}

void AProjector::ProjectVideo()
{
	if (Screen)
	{
		if (SM_Screen)
		{
			SM_Screen->SetMaterial(0, MPMaterial);
			TArray<AActor*> ActorsToFind;
			if (UWorld* World = GetWorld())
			{
				UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AWayPoints::StaticClass(), FName("1"), ActorsToFind);
			}
			for (AActor* WayPointsActor : ActorsToFind)
			{
				WayPointsCast = Cast<AWayPoints>(WayPointsActor);
				if (WayPointsCast)
				{
					WayPointsCast->AISpawnEventFunction();
				}
			}
		}
	}
		
	if(MP && MS)
		MP->OpenSource(MS);
	SpotLight_B->SetVisibility(true);
	SpotLight_W->SetVisibility(true);
}

void AProjector::Reset()
{
	if (MP && MS && MP->IsPlaying())
	{
		MP->Close();
		SM_Screen->SetMaterial(0, DefaultMaterial);
	}
	bDoOncePlay = true;
}