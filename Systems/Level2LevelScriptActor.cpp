#include "Level2LevelScriptActor.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameState.h"
#include "MyGameInstance.h"
#include "Master_Movable.h"
#include "MainCharacter.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void ALevel2LevelScriptActor::BeginPlay()
{
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(false);

	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (MainCharacter)
	{
		MainCharacter->MainCharacterLocation = GetActorLocation();
		MainCharacter->ResetHidedActors();
	}
		
}