/*
* Header "KeyGuideWidget"
* Created by Yang
* Last Modified by Yang
* Description:
* It is a widget that notifies the user of the key.
*/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "KeyGuideWidget.generated.h"

UCLASS()
class CAP2_API UKeyGuideWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* KeyGuideText;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
		class UWidgetAnimation* Fade;
	UFUNCTION()
		void SetKeyGuideText(FString ObjectName);
	UFUNCTION()
		FString CheckWhatKeyGuide(FString ObjectName);

protected:
	virtual void NativeConstruct() override;
};
