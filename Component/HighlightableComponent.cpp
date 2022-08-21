// Fill out your copyright notice in the Description page of Project Settings.


#include "HighlightableComponent.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "CableComponent.h"

// Sets default values for this component's properties
UHighlightableComponent::UHighlightableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
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
	//StaticMesh->SetRenderCustomDepth(true);
	/*if (Cast<UCableComponent>(GetOwner()->GetComponentByClass(UCableComponent::StaticClass())))
	{
		Cable = Cast<UCableComponent>(GetOwner()->GetComponentByClass(UCableComponent::StaticClass()));
	}	*/
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
			
		/*if (Cable)
		{
			UE_LOG(LogTemp, Log, TEXT("123123"));
			Cable->SetRenderCustomDepth(true);
		}*/
			
	}

}

void UHighlightableComponent::DelayFunc()
{
	DoN = 0;
	if (StaticMesh)
		StaticMesh->SetRenderCustomDepth(false);
	/*if(Cable)
		Cable->SetRenderCustomDepth(false);*/
}