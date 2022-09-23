/*
* Source "Master_Slicable"
* Created by Park
* Last Modified by Park
* Modified: 2022.05.14
* Description: Slicable Object Can be Sliced or Droped
*/

#include "Master_Slicable.h"
#include "KismetProceduralMeshLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MainCharacter.h"
#include "MyGameInstance.h"
#include "Master_Movable.h"
#include "SaveGameObject.h"
#include "HighlightableComponent.h"

AMaster_Slicable::AMaster_Slicable()
{
	PrimaryActorTick.bCanEverTick = false;

	SlicableScene = CreateDefaultSubobject<USceneComponent>(TEXT("MasterSlicable_SceneComponent"));
	SlicableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MasterSlicable_StaticMeshComponent"));
	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("StaticMesh"));
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	HandIKLoc = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HandIKLoc"));

	ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Meshes/1M_Cube.1M_Cube'"));
	if (MeshAsset.Succeeded())
	{
		SlicableMesh->SetStaticMesh(MeshAsset.Object);
		SlicableMesh->SetSimulatePhysics(false);
		SlicableMesh->SetVisibility(false);
		SlicableMesh->SetCollisionProfileName("Custom...");
	}

	ProceduralMesh->SetSimulatePhysics(false);
	ProceduralMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ProceduralMesh->bUseComplexAsSimpleCollision = false;
	ProceduralMesh->bUseAsyncCooking = false;
	ProceduralMesh->SetVisibility(true);

	RootComponent = SlicableScene;
	SlicableMesh->SetupAttachment(RootComponent);
	ProceduralMesh->SetupAttachment(RootComponent);
	Box->SetupAttachment(ProceduralMesh);
	HandIKLoc->SetupAttachment(ProceduralMesh);

	Box->OnComponentBeginOverlap.AddDynamic(this, &AMaster_Slicable::OnOverlapBegin);
	Box->OnComponentEndOverlap.AddDynamic(this, &AMaster_Slicable::OnOverlapEnd);

	bIsUsed = false;
	UKismetProceduralMeshLibrary::CopyProceduralMeshFromStaticMeshComponent(SlicableMesh, 0, ProceduralMesh, true);

	HightlightableComponent = CreateDefaultSubobject<UHighlightableComponent>(TEXT("HightlightableComponent"));
}

void AMaster_Slicable::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->OnReset.AddUFunction(this, FName("Reset"));
		GameInstance->OnSave.AddUFunction(this, FName("Save"));
	}
}

// For Slice Half
void AMaster_Slicable::SliceMesh(FVector CharacterForwardVector)
{
	FVector MeshPosition = SlicableMesh->GetComponentLocation();
	FVector Axis;
	FVector ImpulseForce;
	float DotFValue = MainCharacter->GetActorForwardVector().Y * -1;

	abs(CharacterForwardVector.Y) > abs(CharacterForwardVector.X) ? Axis = FVector(1, 0, 0) : Axis = FVector(0, 1, 0);
	ImpulseForce = CharacterForwardVector * -200.0f;

	UMaterialInterface* MeshMaterial = SlicableMesh->GetMaterial(0);
	UKismetProceduralMeshLibrary::SliceProceduralMesh(ProceduralMesh, MeshPosition, Axis, true, OutHalfMesh, EProcMeshSliceCapOption::CreateNewSectionForCap, MeshMaterial);
	OutHalfMesh->SetSimulatePhysics(true);
	ProceduralMesh->SetSimulatePhysics(true);
	ProceduralMesh->AddImpulse(ImpulseForce + FVector(-400, DotFValue * 800, 0), NAME_None, true);
	OutHalfMesh->AddImpulse(ImpulseForce + FVector(400, DotFValue * 800, 0), NAME_None, true);

	bIsUsed = true;

	SlicableMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// For Drop
void AMaster_Slicable::DropMesh(FVector CharacterForwardVector)
{
	FVector Axis = FVector(CharacterForwardVector.X, CharacterForwardVector.Y, -0.5f);
	FVector ImpulseForce;
	ImpulseForce = Axis* -500.0f;
	ProceduralMesh->SetSimulatePhysics(true);
	ProceduralMesh->AddImpulse(ImpulseForce, NAME_None, true);
	bIsUsed = true;
}

void AMaster_Slicable::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	MainCharacter = Cast<AMainCharacter>(OtherActor);
	if (MainCharacter)
	{
		MainCharacter->bIsOverlapWithBoard = true;
	}
}

void AMaster_Slicable::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (MainCharacter)
	{
		MainCharacter->bIsOverlapWithBoard = false;
	}
}

void AMaster_Slicable::Reset()
{
	if (GameInstance->SaveGameData->Level1SaveDataStruct.SlicableData.Contains(this))
	{
		bIsUsed = *GameInstance->SaveGameData->Level1SaveDataStruct.SlicableData.Find(this);
		if (bIsUsed)
		{
			Destroy();
		}
	}
}
void AMaster_Slicable::Save()
{
	GameInstance->SaveGameData->Level1SaveDataStruct.SlicableData.Add(this, bIsUsed);
}

