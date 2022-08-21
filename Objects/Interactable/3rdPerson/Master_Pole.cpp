#include "Master_Pole.h"
#include "MainCharacter.h"
#include "HighlightableComponent.h"

AMaster_Pole::AMaster_Pole()
{
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetCollisionProfileName(FName("Custom..."));
	StaticMesh->SetupAttachment(RootComponent);

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(RootComponent);

	HightlightableComponent = CreateDefaultSubobject<UHighlightableComponent>(TEXT("HightlightableComponent"));

	Box->OnComponentBeginOverlap.AddDynamic(this, &AMaster_Pole::OnOverlapBegin);
}

void AMaster_Pole::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	MainCharacter = Cast<AMainCharacter>(OtherActor);
	if (MainCharacter)
	{
		//MainCharacter->PoleClimb();
	}
}
