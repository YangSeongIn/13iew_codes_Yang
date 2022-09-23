#include "Trigger_FloorBreak.h"

ATrigger_FloorBreak::ATrigger_FloorBreak()
{
	PrimaryActorTick.bCanEverTick = true;
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATrigger_FloorBreak::OnCapsuleComponentOverlapBegin);
}

void ATrigger_FloorBreak::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATrigger_FloorBreak::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ATrigger_FloorBreak::OnCapsuleComponentOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void ATrigger_FloorBreak::OnCapsuleComponentOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}