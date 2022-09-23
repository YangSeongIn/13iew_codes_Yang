#include "Master_Movable.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGameObject.h"
#include "MainCharacter.h"
#include "Components/BoxComponent.h"
#include "HighlightableComponent.h"

AMaster_Movable::AMaster_Movable()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(StaticMesh);
	Box->SetRelativeLocation(FVector(0, 0, 50));

	HightlightableComponent = CreateDefaultSubobject<UHighlightableComponent>(TEXT("HightlightableComponent"));

	DefaultBoxLocation = Box->GetRelativeLocation();
}

void AMaster_Movable::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->OnReset.AddUFunction(this, FName("Reset"));
		GameInstance->OnSave.AddUFunction(this, FName("Save"));
	}
	InitializeBox();
}

void AMaster_Movable::Reset()
{
	if (GameInstance->SaveGameData->Level1SaveDataStruct.MovableData.Contains(this))
	{
		SetActorTransform(*GameInstance->SaveGameData->Level1SaveDataStruct.MovableData.Find(this));
	}
}

void AMaster_Movable::Save()
{
	GameInstance->SaveGameData->Level1SaveDataStruct.MovableData.Add(this, GetActorTransform());
}

void AMaster_Movable::InitializeBox()
{
	Box->SetCollisionProfileName(TEXT("NoCollision"));
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Box->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
}