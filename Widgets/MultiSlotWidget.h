/*
* Header "MultiSlotWidget"
* Created by Yang
* Last Modified by cho
* Description:
* A widget to manage the back button, settings button, and each slot button.
*/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "MultiSlotWidget.generated.h"

UCLASS()
class CAP2_API UMultiSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
		TArray<UUserWidget*> Widgets;

	UPROPERTY(meta = (BindWidget))
		UButton* BackButton;

	UPROPERTY(meta = (BindWidget))
		class USlotWidget* Widget_Slot_1;
	UPROPERTY(meta = (BindWidget))
		class USlotWidget* Widget_Slot_2;
	UPROPERTY(meta = (BindWidget))
		class USlotWidget* Widget_Slot_3;

	// test
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* BackButtonText;
	UFUNCTION()
		void OnHovered();
	UFUNCTION()
		void OnUnHovered();
	UFUNCTION()
		void BackToMainMenu();


	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> _MainMenuWidget;
	UPROPERTY()
		class UMainMenuWidget* MainMenuWidget;

	//test
	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> _DetailWidget;
	class USlotDetailWidget* DetailWidget;



protected:
	virtual void NativeConstruct() override;
};
