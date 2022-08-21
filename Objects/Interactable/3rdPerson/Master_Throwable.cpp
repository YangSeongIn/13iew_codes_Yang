#include "Master_Throwable.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Components/BoxComponent.h"
#include "Master_Movable.h"
#include "HighlightableComponent.h"

#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "UObject/ConstructorHelpers.h"
#include "MainCharacter.h"
#include "AI_Tags.h"
#include "SaveGameObject.h"
#include "Components/BoxComponent.h"
#include "MyGameInstance.h"

#include "MainCharacter.h"
// Sets default values
AMaster_Throwable::AMaster_Throwable()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;
	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->CanCharacterStepUp(false);
	StaticMesh->SetCollisionProfileName(FName("Custom..."));
	StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_EngineTraceChannel5, ECollisionResponse::ECR_Block);
	HightlightableComponent = CreateDefaultSubobject<UHighlightableComponent>(TEXT("HightlightableComponent"));

	SetupStimulus();
}

void AMaster_Throwable::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->OnReset.AddUFunction(this, FName("Reset"));
		GameInstance->OnSave.AddUFunction(this, FName("Save"));
	}
}


void AMaster_Throwable::DropDown()
{
	Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->bIsHoldingThrowable = false;
	StaticMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	StaticMesh->SetSimulatePhysics(true);
	FTimerHandle WaitHandle;
	float WaitTime = 1.0f;
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

		}), WaitTime, false);
}

void AMaster_Throwable::PickUp(FVector PickUpLocation, class AMainCharacter* MainCharacter)
{
	StaticMesh->SetSimulatePhysics(false);
	StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	FLatentActionInfo Info;
	Info.CallbackTarget = this;
	Info.Linkage = 0;
	Info.ExecutionFunction = FName("MoveComponentToFunction");
	
	UKismetSystemLibrary::MoveComponentTo(
		StaticMesh, 
		PickUpLocation,
		FRotator(0.0f, 0.0f, 0.0f), 
		true, 
		true,
		0.2f,
		false, 
		EMoveComponentAction::Type::Move, 
		Info
	);
	//isHit = false;
}

void AMaster_Throwable::Throw(class AMainCharacter* MainCharacter, float ChargeRate)
{
	MainCharacter->bIsHoldingThrowable = false;
	MainCharacter->PlayAnimMontage(MainCharacter->M_Throw);
	MainCharacter->SetIsControllable(false);
	StaticMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	StaticMesh->SetSimulatePhysics(true);
	FVector ThrowPower = (MainCharacter->TriggerCapsuleComponent->GetForwardVector() * 100.0f + MainCharacter->TriggerCapsuleComponent->GetUpVector() * 300.0f) * ChargeRate * ThrowPowerFloat;
	StaticMesh->AddImpulse(ThrowPower);

	FTimerHandle WaitHandle;
	float WaitTime = 0.2f;
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
		}), WaitTime, false);
}

void AMaster_Throwable::SetupStimulus()
{
	stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("stimulus"));
	stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	stimulus->RegisterWithPerceptionSystem();
}
void AMaster_Throwable::MoveComponentToFunction()
{
	StaticMesh->AttachToComponent(Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->SkeletalMesh, FAttachmentTransformRules::KeepWorldTransform, FName("hand_r"));
}

void AMaster_Throwable::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMaster_Movable* Movable = Cast<AMaster_Movable>(OtherActor);
	if (Movable)
	{
		AttachToActor(Movable, FAttachmentTransformRules::KeepWorldTransform);
		bIsAttached = true;
	}
}
// OnCollisionEnd
void AMaster_Throwable::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bIsAttached)
	{
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		bIsAttached = false;
	}
}

void AMaster_Throwable::Reset()
{
	if (GameInstance->SaveGameData->Level1SaveDataStruct.ThrowableData.Contains(this))
	{
		SetActorTransform(*GameInstance->SaveGameData->Level1SaveDataStruct.ThrowableData.Find(this));
	}
}

void AMaster_Throwable::Save()
{
	GameInstance->SaveGameData->Level1SaveDataStruct.ThrowableData.Add(this, GetActorTransform());
}

