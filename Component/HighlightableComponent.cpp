#include "HighlightableComponent.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "CableComponent.h"

UHighlightableComponent::UHighlightableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UHighlightableComponent::BeginPlay()
{
	Super::BeginPlay();

	MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass())))
	{
		StaticMesh = Cast<UStaticMeshComponent>(GetOwner()->GetDefaultSubobjectByName(FName(TEXT("StaticMesh"))));
	}
}

// Called every frame
void UHighlightableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UHighlightableComponent::TurnOnHighlight()
{
	LAI.CallbackTarget = this;
	LAI.ExecutionFunction = "DelayFunc";
	LAI.Linkage = 0;
	UKismetSystemLibrary::RetriggerableDelay(GetWorld(), 0.1f, LAI);
	while (DoN < 1)
		DoN++;
	{
		if (StaticMesh)
		{
			StaticMesh->SetRenderCustomDepth(true);
		}			
	}

}

void UHighlightableComponent::DelayFunc()
{
	DoN = 0;
	if (StaticMesh)
		StaticMesh->SetRenderCustomDepth(false);
}