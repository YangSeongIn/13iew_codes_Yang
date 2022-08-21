// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableItem_Key.h"
#include "MainCharacter.h"

AInteractableItem_Key::AInteractableItem_Key()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
}

void AInteractableItem_Key::BeginPlay()
{
	Super::BeginPlay();

	MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void AInteractableItem_Key::Interact()
{
	MainCharacter->Inventory.Add(this);
	Destroy();
}