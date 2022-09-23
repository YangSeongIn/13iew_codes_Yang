#include "Cap2GameModeBase.h"
#include "MainCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Master_Movable.h"
#include "MyGameInstance.h"
#include "Level1LevelScriptActor.h"
#include "CheckPoint.h"
#include "FirstPersonDefaultWidget.h"

void ACap2GameModeBase::BeginPlay()
{
	MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (MainCharacter)
	{
		MainCharacter->OnDie.AddUFunction(this, FName("EventOnMainCharacterDie"));
	}
}

void ACap2GameModeBase::EventOnMainCharacterDie()
{
	if (GetWorld())
	{
		FTimerHandle WaitHandle;
		RespawnTime = MainCharacter->DefaultWidget->FadeIn->GetEndTime();
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			GameInstance->LoadData();
		}), RespawnTime, false);
	}
}