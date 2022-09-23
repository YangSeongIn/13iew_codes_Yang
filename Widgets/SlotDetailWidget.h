/*
* Header "SlotDetailWidget"
* Created by Yang
* Last Modified by Yang
* Description:
* Widget that displays save data information that appears when a SlotWidget is clicked.
*/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlotDetailWidget.generated.h"

UCLASS()
class CAP2_API USlotDetailWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	class UMyGameInstance* GameInstance;

	int32 SlotIndex;
	FString SavedPoint;
	float TotalPlayTime;

	UPROPERTY(BlueprintReadWrite)
		FString SlotName;


	UFUNCTION()
		void SetDeleteButtonEnabled();

	UPROPERTY(meta = (BindWidget))
		class UImage* BackGroundImage;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* DetailText;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SlotLevel;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PlayedTime;

	UPROPERTY(meta = (BindWidget))
		class UButton* StartButton;
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
		class UButton* DeleteButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* CloseButton;


	UFUNCTION()
		void StartGame();
	UFUNCTION()
		void DeleteGame();
	UFUNCTION()
		void CloseWidget();

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> _DeleteCheckWidget;
	class USlotDeleteCheckWidget* DeleteCheckWidget;


	void SetTexture();


protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;
};
