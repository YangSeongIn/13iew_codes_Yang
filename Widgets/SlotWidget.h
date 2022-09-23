/*
* Header "SlotWidget"
* Created by Yang
* Last Modified by Yang
* Description:
* Each slot that appears in the multi-slot menu. Total 3 slots.
*/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

#include "SlotWidget.generated.h"

DECLARE_DELEGATE(FOnClickStartGame);

UCLASS()
class CAP2_API USlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	class UMyGameInstance* GameInstance;

	UPROPERTY(meta = (BindWidget))
		class UButton* SlotButton;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SlotNameText;

	UPROPERTY()
		int8 SlotIndex;
	UPROPERTY()
		FString SlotName;
	UPROPERTY()
		FString SavedPoint;

	UFUNCTION()
		void StartSlot();
	UFUNCTION()
		void SetSlotInfoText();


	// test
	UFUNCTION()
		void OnHovered();
	UFUNCTION()
		void OnUnHovered();

	FString FormatTimeToHHMMSS(float t);



	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> _DetailWidget;
	class USlotDetailWidget* DetailWidget;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> _MainMenuWidget;
	class UMainMenuWidget* MainMenuWidget;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> _SettingWidget;
	class USettingWidget* SettingWidget;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> _CreditWidget;
	class UCreditWidget* CreditWidget;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> _MultiSlotWidget;
	class UMultiSlotWidget* MultiSlotWidget;



	UPROPERTY()
		class USaveGameObject* SaveGameData;

	FOnClickStartGame OnClickStartGame;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;
};
