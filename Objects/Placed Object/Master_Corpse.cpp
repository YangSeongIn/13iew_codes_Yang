#include "Master_Corpse.h"

AMaster_Corpse::AMaster_Corpse()
{
	PrimaryActorTick.bCanEverTick = false;
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    RootComponent = StaticMesh;
    
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CorpseAsset (TEXT(
        "StaticMesh'/Game/Asset/Horror_Props/Mesh/SM_Dead_Man.SM_Dead_Man'"));
    static ConstructorHelpers::FObjectFinder<UMaterial> CorpseMaterial (TEXT(
        "Material'/Game/Horror_Props/Materials/M_Man_in_bag.M_Man_in_bag'"));
    if (CorpseAsset.Succeeded() && CorpseMaterial.Succeeded())
    {
        StaticMesh->SetStaticMesh(CorpseAsset.Object);
        StaticMesh->SetMaterial(0, CorpseMaterial.Object);
    }

    StaticMesh->SetSimulatePhysics(true);
    StaticMesh->BodyInstance.bOverrideMass = true;
    StaticMesh->BodyInstance.SetMassOverride(2000);
    StaticMesh->SetRelativeScale3D(FVector(1.7f));
}


