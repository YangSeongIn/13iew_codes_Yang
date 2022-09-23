#include "Elevator.h"
#include "Kismet/KismetSystemLibrary.h"
#include "InteractableObject_ElevatorBtn.h"

AElevator::AElevator()
{
	PrimaryActorTick.bCanEverTick = true;

	Frame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Frame"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FrameAsset(TEXT("StaticMesh'/Game/StaticMesh/Mesh/Elevator_Cube_001.Elevator_Cube_001'"));
	if (FrameAsset.Succeeded())
	{
		Frame->SetStaticMesh(FrameAsset.Object);
		RootComponent = Frame;
	}
	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DoorAsset(TEXT("StaticMesh'/Game/StaticMesh/Mesh/SM_ElevatorDoor.SM_ElevatorDoor'"));
	if (FrameAsset.Succeeded())
	{
		Door->SetStaticMesh(DoorAsset.Object);
		Door->SetupAttachment(Frame);
	}

	Frame->SetRelativeScale3D(FVector(0.75f, 1, 1));
	Door->SetRelativeLocation(FVector(-106.6f, -10.83f, -260));
	Door->SetRelativeRotation(FRotator(0, -90, 0));
	Door->SetRelativeScale3D(FVector(1, 1, 1.25f));
}

void AElevator::BeginPlay()
{
	Super::BeginPlay();
	
}

void AElevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AElevator::Operate(AInteractableObject_ElevatorBtn* Btn)
{
	Btn->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform, NAME_None);
	FLatentActionInfo LAI;
	LAI.CallbackTarget = this;
	LAI.ExecutionFunction = "Elevate";
	LAI.Linkage = 0;

	UKismetSystemLibrary::MoveComponentTo(
		Door,
		FVector(-106.6f, -10.83f, DoorMoveLength),
		Door->GetRelativeRotation(),
		true,
		true,
		3.0f,
		false,
		EMoveComponentAction::Move,
		LAI
	);
}

void AElevator::Elevate()
{
	FLatentActionInfo LAI;
	LAI.CallbackTarget = this;
	LAI.Linkage = 0;
	UKismetSystemLibrary::MoveComponentTo(
		Frame,
		Frame->GetRelativeLocation() + FVector(0, 0, ElevatorMoveLength),
		Frame->GetRelativeRotation(),
		true,
		true,
		5,
		false,
		EMoveComponentAction::Move,
		LAI
	);
}