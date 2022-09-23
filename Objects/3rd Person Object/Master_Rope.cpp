#include "Master_Rope.h"
#include "HighlightableComponent.h"

AMaster_Rope::AMaster_Rope()
{
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	Cable = CreateDefaultSubobject<UCableComponent>(TEXT("Cable"));
	Cable->SetupAttachment(RootComponent);
	Cable->SolverIterations = 5;

	CableEnd = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CableEnd"));
	CableEnd->SetupAttachment(RootComponent);
	CableEnd->GetBodyInstance()->bLockZRotation = true;

	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint"));
	PhysicsConstraint->SetupAttachment(RootComponent);

	HightlightableComponent = CreateDefaultSubobject<UHighlightableComponent>(TEXT("HightlightableComponent"));

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetupAttachment(CableEnd);
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(CapsuleComponent);

	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AMaster_Rope::OnCapsuleComponentOverlapBegin);
	CapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &AMaster_Rope::OnCapsuleComponentOverlapEnd);
}

void AMaster_Rope::OnCapsuleComponentOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Interface_MainCharacterInfo = Cast<IInterface_MainCharacterInfo>(OtherActor);
	if (Interface_MainCharacterInfo)
	{
		if (Interface_MainCharacterInfo->IsMainCharacter() && Interface_MainCharacterInfo->GetIsPressLMB() && !Interface_MainCharacterInfo->GetIsHangingRope())
		{
			if (bIsOneSideRope)
			{
				if (UKismetMathLibrary::NearlyEqual_FloatFloat(FVector::DotProduct(GetActorForwardVector(), OtherActor->GetActorForwardVector()), 1, 0.001) || UKismetMathLibrary::NearlyEqual_FloatFloat(FVector::DotProduct(GetActorForwardVector(), OtherActor->GetActorForwardVector()), -1, 0.001))
				{
					Interface_MainCharacterInfo->GrapRope(this, HangingLocZ);
				}
			}
			else
			{
				Interface_MainCharacterInfo->GrapRope(this, HangingLocZ);
			}
		}
	}
}

void AMaster_Rope::OnCapsuleComponentOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Interface_MainCharacterInfo = nullptr;
}