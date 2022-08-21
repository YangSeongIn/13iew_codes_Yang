// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyGuideTrigger.h"
#include "KeyGuideWidget.h"

AKeyGuideTrigger::AKeyGuideTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
}

void AKeyGuideTrigger::BeginPlay()
{
	Super::BeginPlay();

	Box->OnComponentBeginOverlap.AddDynamic(this, &AKeyGuideTrigger::OnOverlapBegin);
}

void AKeyGuideTrigger::ShowKeyGuideOnWidget()
{
	KeyGuideWidget = Cast<UKeyGuideWidget>(CreateWidget(GetWorld(), _KeyGuideWidget));
	if (KeyGuideWidget)
	{
		KeyGuideWidget->SetKeyGuideText(GimmickName);
		KeyGuideWidget->AddToViewport();
	}
	bDoOnce = false;

	FTimerHandle WaitHandle;
	float WaitTime = KeepingTime;
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			KeyGuideWidget->RemoveFromParent();
		}), WaitTime, false);
}

void AKeyGuideTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bDoOnce && _KeyGuideWidget)
	{
		ShowKeyGuideOnWidget();
	}
}

// OnCollisionEnd
void AKeyGuideTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}