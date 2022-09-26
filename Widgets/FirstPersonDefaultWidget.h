/*
* Header "FirstPersonDefaultWidget"
* Created by Yang
* Last Modified by Yang
* Description:
* All of Default widget animaion & image.
*/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "FirstPersonDefaultWidget.generated.h"

UCLASS()
class CAP2_API UFirstPersonDefaultWidget : public UUserWidget
{
	GENERATED_BODY()

private:


public:
	class AMainCharacter* MainCharacter;
	FString NextLevelName = "";
	bool bIsToNextLevel;

	void PlayWidgetAnimation(UWidgetAnimation* Animation, bool b, FString LevelName);

	UPROPERTY(meta = (BindWidget))
		class UImage* Image_CanSwitchView;
	UPROPERTY(meta = (BindWidget))
		class UImage* FP_Point;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
		class UWidgetAnimation* FadeIn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetAnim), Transient)
		class UWidgetAnimation* FadeOut;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
		class UWidgetAnimation* FadeIn_Doctor;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
		class UWidgetAnimation* FadeOut_Doctor;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
		class UWidgetAnimation* Controllable;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
		class UWidgetAnimation* UnControllable;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
		class UWidgetAnimation* Save;

	FWidgetAnimationDynamicEvent FadeInEndDelegate;
	FWidgetAnimationDynamicEvent FadeOutStartDelegate;
	FWidgetAnimationDynamicEvent FadeOutEndDelegate;
	FWidgetAnimationDynamicEvent FadeIn_DoctorEndDelegate;
	FWidgetAnimationDynamicEvent FadeOut_DoctorStartDelegate;
	FWidgetAnimationDynamicEvent FadeOut_DoctorEndDelegate;
	FWidgetAnimationDynamicEvent ControllableEndDelegate;
	FWidgetAnimationDynamicEvent UnControllableStartDelegate;

	UFUNCTION()
		void FadeOutAnimationStarted();
	UFUNCTION()
		void FadeOutAnimationEnded();
	UFUNCTION()
		void FadeInAnimationFinished();
	UFUNCTION()
		void FadeOut_DoctorAnimationStarted();
	UFUNCTION()
		void FadeOut_DoctorAnimationEnded();
	UFUNCTION()
		void FadeIn_DoctorAnimationFinished();
	UFUNCTION()
		void UnControllableAnimationStarted();
	UFUNCTION()
		void ControllableAnimationFinished();

protected:
	virtual void NativeConstruct() override;
};
