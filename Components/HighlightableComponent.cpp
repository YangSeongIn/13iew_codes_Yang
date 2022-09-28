#include "HighlightableComponent.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "CableComponent.h"

void UHighlightableComponent::BeginPlay()
{
	Super::BeginPlay();

	MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	bool bCastStaticMesh = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	if (bCastStaticMesh)
	{
		StaticMesh = Cast<UStaticMeshComponent>(GetOwner()->GetDefaultSubobjectByName(FName(TEXT("StaticMesh"))));
	}
}

void UHighlightableComponent::TurnOnHighlight()
{
	// RetriggerableDelay LatentActionInfo Struct for highlighting for a  certain period of time
	FLatentActionInfo LAI;
	LAI.CallbackTarget = this;
	LAI.ExecutionFunction = "DelayFunc";
	LAI.Linkage = 0;

	float DeleteHighlightingTime = 0.1f;
	UKismetSystemLibrary::RetriggerableDelay(GetWorld(), DeleteHighlightingTime, LAI);
	while (DoN < 1)
	{
		DoN++;
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