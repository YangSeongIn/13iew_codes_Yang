#include "Rope_NormalRope.h"

ARope_NormalRope::ARope_NormalRope()
{
	Cable->CableLength = 200.0f;

	Cable->SetCollisionProfileName(FName("Custom..."));
	Cable->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	CableEnd->SetRelativeLocation(FVector(0.0f, 0.0f, -300.0f));
	CableEnd->SetSimulatePhysics(true);
	CableEnd->SetLinearDamping(0.2f);
	CableEnd->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
	CableEnd->SetCollisionProfileName(FName("Custom..."));
	CableEnd->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	CableEnd->bHiddenInGame = true;

	Cable->AttachEndTo.ComponentProperty = FName("CableEnd");
	Cable->bAttachEnd = true;
	Cable->EndLocation = FVector(0.0f);

	PhysicsConstraint->ComponentName1.ComponentName = FName("DefaultSceneRoot");
	PhysicsConstraint->ComponentName2.ComponentName = FName("CableEnd");

	CapsuleComponent->SetRelativeScale3D(FVector(5.0f, 5.0f, 5.0f));
}

void ARope_NormalRope::AddSwingForce(FVector SwingVector)
{
	if (GetActorLocation().Z - CableEnd->GetComponentLocation().Z > bIsAbleToSwingZ)
	{
		CableEnd->AddForce(SwingVector * 100.0f, NAME_None, true);
	}

}