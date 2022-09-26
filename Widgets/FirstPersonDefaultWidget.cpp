#include "FirstPersonDefaultWidget.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"
#include "SaveGameObject.h"

void UFirstPersonDefaultWidget::NativeConstruct()
{
	FadeOutStartDelegate.BindDynamic(this, &UFirstPersonDefaultWidget::FadeOutAnimationStarted);
	FadeOutEndDelegate.BindDynamic(this, &UFirstPersonDefaultWidget::FadeOutAnimationEnded);
	FadeInEndDelegate.BindDynamic(this, &UFirstPersonDefaultWidget::FadeInAnimationFinished);

	FadeOut_DoctorStartDelegate.BindDynamic(this, &UFirstPersonDefaultWidget::FadeOut_DoctorAnimationStarted);
	FadeOut_DoctorEndDelegate.BindDynamic(this, &UFirstPersonDefaultWidget::FadeOut_DoctorAnimationEnded);
	FadeIn_DoctorEndDelegate.BindDynamic(this, &UFirstPersonDefaultWidget::FadeIn_DoctorAnimationFinished);

	UnControllableStartDelegate.BindDynamic(this, &UFirstPersonDefaultWidget::UnControllableAnimationStarted);
	ControllableEndDelegate.BindDynamic(this, &UFirstPersonDefaultWidget::ControllableAnimationFinished);

	BindToAnimationFinished(FadeIn, FadeInEndDelegate);
	BindToAnimationStarted(FadeOut, FadeOutStartDelegate);
	BindToAnimationFinished(FadeOut, FadeOutEndDelegate);

	BindToAnimationFinished(FadeIn_Doctor, FadeIn_DoctorEndDelegate);
	BindToAnimationStarted(FadeOut_Doctor, FadeOut_DoctorStartDelegate);
	BindToAnimationFinished(FadeOut_Doctor, FadeOut_DoctorEndDelegate);

	BindToAnimationFinished(Controllable, ControllableEndDelegate);
	BindToAnimationStarted(UnControllable, UnControllableStartDelegate);

	MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void UFirstPersonDefaultWidget::FadeOutAnimationStarted()
{
	PlayAnimation(UnControllable);
}

void UFirstPersonDefaultWidget::FadeOutAnimationEnded()
{
	if (bIsToNextLevel)
	{
		bIsToNextLevel = false;
		UGameplayStatics::OpenLevel(GetWorld(), FName(NextLevelName));
	}
}

void UFirstPersonDefaultWidget::FadeInAnimationFinished()
{
	//if(MainCharacter->bIsTestingMode)
	//	PlayAnimation(Controllable);
}

void UFirstPersonDefaultWidget::FadeOut_DoctorAnimationStarted()
{
	PlayAnimation(UnControllable);
}

void UFirstPersonDefaultWidget::FadeOut_DoctorAnimationEnded()
{
	if (bIsToNextLevel)
	{
		bIsToNextLevel = false;
		UGameplayStatics::OpenLevel(GetWorld(), FName(NextLevelName));
	}
}

void UFirstPersonDefaultWidget::FadeIn_DoctorAnimationFinished()
{
	if (MainCharacter->bIsTestingMode)
		PlayAnimation(Controllable);
}

void UFirstPersonDefaultWidget::UnControllableAnimationStarted()
{
	MainCharacter->SetIsControllable(false);
}

void UFirstPersonDefaultWidget::ControllableAnimationFinished()
{
	MainCharacter->SetIsControllable(true);
}

void UFirstPersonDefaultWidget::PlayWidgetAnimation(UWidgetAnimation* Animation, bool b, FString LevelName)
{
	PlayAnimation(Animation);
	bIsToNextLevel = b;
	NextLevelName = LevelName;
}