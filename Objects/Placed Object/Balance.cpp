#include "Balance.h"
#include "MainCharacter.h"
#include "Master_Throwable.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


ABalance::ABalance()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(DefaultSceneRoot);
	Body->SetRelativeScale3D(FVector(2.0f, 2.5f, 0.25f));

	Body_Child = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body_Child"));
	Body_Child->SetupAttachment(Body);
	Body_Child->SetRelativeScale3D(FVector(0.125f, 0.1f, 6.0f));

	Arm = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arm"));
	Arm->SetupAttachment(DefaultSceneRoot);
	Arm->SetRelativeLocation(FVector(25.0f, 0.0f, 143.0f));
	Arm->SetRelativeScale3D(FVector(0.25f, 4.5f, 0.25f));
	Arm->BodyInstance.bLockTranslation = true;
	Arm->BodyInstance.bLockYRotation = true;
	Arm->BodyInstance.bLockZRotation = true;

	Plate1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plate1"));
	Plate1->SetupAttachment(Arm);
	Plate1->SetRelativeLocation(FVector(120.0f, 45.0f, 240.0f));
	Plate1->SetRelativeScale3D(FVector(3.0f, 0.15f, 2.5f));

	Plate2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plate2"));
	Plate2->SetupAttachment(Arm);
	Plate2->SetRelativeLocation(FVector(120.0f, -45.0f, 240.0f));
	Plate2->SetRelativeScale3D(FVector(3.0f, 0.15f, 2.5f));

	Box1 = CreateDefaultSubobject<UBoxComponent>(TEXT("Box1"));
	Box1->SetupAttachment(Plate1);
	Box1->SetRelativeLocation(FVector(0.0f, 0.0f, 40.0f));
	Box1->SetRelativeScale3D(FVector(3.8f, 3.8f, 1.0f));

	Box2 = CreateDefaultSubobject<UBoxComponent>(TEXT("Box2"));
	Box2->SetupAttachment(Plate2);
	Box2->SetRelativeLocation(FVector(0.0f, 0.0f, 40.0f));
	Box2->SetRelativeScale3D(FVector(3.8f, 3.8f, 1.0f));
	
	PhysicsConstraint1 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint1"));
	PhysicsConstraint1->SetupAttachment(DefaultSceneRoot);
	PhysicsConstraint1->SetRelativeLocation(FVector(0.0f, 210.0f, 136.0f));
	PhysicsConstraint1->ComponentName1.ComponentName = FName("Arm");
	PhysicsConstraint1->ComponentName2.ComponentName = FName("Plate1");
	PhysicsConstraint1->SetDisableCollision(true);
	PhysicsConstraint1->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.0f);
	PhysicsConstraint1->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.0f);

	PhysicsConstraint2 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraint2"));
	PhysicsConstraint2->SetupAttachment(DefaultSceneRoot);
	PhysicsConstraint2->SetRelativeLocation(FVector(0.0f, -210.0f, 136.0f));
	PhysicsConstraint2->ComponentName1.ComponentName = FName("Arm");
	PhysicsConstraint2->ComponentName2.ComponentName = FName("Plate2");
	PhysicsConstraint2->SetDisableCollision(true);
	PhysicsConstraint2->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.0f);
	PhysicsConstraint2->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.0f);

	
}

void ABalance::BeginPlay()
{
	Super::BeginPlay();
	
	Box1->OnComponentBeginOverlap.AddDynamic(this, &ABalance::OnBox1OverlapBegin);
	Box1->OnComponentEndOverlap.AddDynamic(this, &ABalance::OnBox1OverlapEnd);
	Box2->OnComponentBeginOverlap.AddDynamic(this, &ABalance::OnBox2OverlapBegin);
	Box2->OnComponentEndOverlap.AddDynamic(this, &ABalance::OnBox2OverlapEnd);

	MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void ABalance::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (MainCharacter && MainCharacter->bIsHoldingThrowable)
	{
		if (bIsMainCharacterOnPlate1)
		{
			Plate1Mass += ThrowableMass;
		}
		else if (bIsMainCharacterOnPlate2)
		{
			Plate2Mass += ThrowableMass;
		}
	}*/
	float FixedMassGapRange = UKismetMathLibrary::Clamp(Plate1Mass - Plate2Mass, -MassGap, MassGap);
	float NormalizedRange = FixedMassGapRange * (20 / MassGap);
	float InterpSpeed = 2.0f;
	float InterpToRange = UKismetMathLibrary::FInterpTo(CurrentArmRotation, NormalizedRange, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), InterpSpeed);
	Arm->SetRelativeRotation(FRotator(0.0f, 0.0f, InterpToRange));
	CurrentArmRotation = InterpToRange;
	Plate1->SetRelativeRotation(FRotator(0.0f, 0.0f, CurrentArmRotation * -1));
	Plate2->SetRelativeRotation(FRotator(0.0f, 0.0f, CurrentArmRotation * -1));
}

float ABalance::AddMassOnPlate(float Mass, AActor* OtherActor)
{
	//AMainCharacter* MC = Cast<AMainCharacter>(OtherActor);
	if (Cast<AMainCharacter>(OtherActor))
	{
		Mass += MainCharacterMass;
	}
	else if (Cast<AMaster_Throwable>(OtherActor))
	{
		Mass += ThrowableMass;
	}

	return Mass;
}

float ABalance::SubMassOnPlate(float Mass, AActor* OtherActor)
{
	//AMainCharacter* MC = Cast<AMainCharacter>(OtherActor);
	if (Cast<AMainCharacter>(OtherActor))
	{
		Mass -= MainCharacterMass;
	}
	else if (Cast<AMaster_Throwable>(OtherActor))
	{
		Mass -= ThrowableMass;
	}

	return Mass;
}

void ABalance::OnBox1OverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Plate1Mass = AddMassOnPlate(Plate1Mass, OtherActor);
}

void ABalance::OnBox1OverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Plate1Mass = SubMassOnPlate(Plate1Mass, OtherActor);
}

void ABalance::OnBox2OverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Plate2Mass = AddMassOnPlate(Plate2Mass, OtherActor);
}

void ABalance::OnBox2OverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Plate2Mass = SubMassOnPlate(Plate2Mass, OtherActor);
}