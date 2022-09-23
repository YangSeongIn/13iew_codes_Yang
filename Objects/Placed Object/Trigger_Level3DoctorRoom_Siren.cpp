#include "Trigger_Level3DoctorRoom_Siren.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"

ATrigger_Level3DoctorRoom_Siren::ATrigger_Level3DoctorRoom_Siren()
{
	PrimaryActorTick.bCanEverTick = false;
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATrigger_Level3DoctorRoom_Siren::OnCapsuleComponentOverlapBegin);

	RectLight = CreateDefaultSubobject<URectLightComponent>("RectLight");
	RectLight->SetupAttachment(RootComponent);
	RectLight->SetIntensity(1000);
	RectLight->LightColor = FColor::Red;
	RectLight->SetAttenuationRadius(5000);
	RectLight->SetSourceWidth(5000);
	RectLight->SetSourceHeight(900);
	RectLight->BarnDoorAngle = 0;
	RectLight->BarnDoorLength = 600;
}

void ATrigger_Level3DoctorRoom_Siren::BeginPlay()
{
	Super::BeginPlay();
	Delegate.BindUFunction(this, "ActivateRectLight", this);
}

void ATrigger_Level3DoctorRoom_Siren::ActivateRectLight()
{
	if (RectLight->IsVisible())
	{
		RectLight->SetVisibility(false);
	}	
	else
	{
		RectLight->SetVisibility(true);
	}
	GEngine->AddOnScreenDebugMessage(
		-1,
		4,
		FColor::Cyan,
		TEXT("ActivateLight")
	);
}

void ATrigger_Level3DoctorRoom_Siren::OnCapsuleComponentOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UWorld* World = GetWorld();
	AMainCharacter* MC = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
	if (!MC) return;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Delay, Delegate, TimeDelay, true);
}

void ATrigger_Level3DoctorRoom_Siren::OnCapsuleComponentOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

