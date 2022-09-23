// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObject_ElevatorBtn.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Elevator.h"

AInteractableObject_ElevatorBtn::AInteractableObject_ElevatorBtn()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
}

void AInteractableObject_ElevatorBtn::Interact()
{
	if (!Elevator) return;
	AElevator* E = Cast<AElevator>(Elevator);
	E->Operate(this);
}
