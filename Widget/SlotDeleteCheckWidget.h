// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SlotDeleteCheckWidget.generated.h"

/**
 * 
 */
UCLASS()
class CAP2_API USlotDeleteCheckWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FString SlotName;

	UPROPERTY(meta = (BindWidget))
		class UButton* DeleteButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* CloseButton;

	UFUNCTION()
		void DeleteGame();
	UFUNCTION()
		void CloseWidget();

	class USlotDetailWidget* DetailWidget;

protected:
	virtual void NativeConstruct() override;
};
