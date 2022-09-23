// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndingComputerScreen3DWidget.generated.h"

UCLASS()
class CAP2_API UEndingComputerScreen3DWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Time_11th;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Time_12th;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Time_13th;

	class UMyGameInstance* GameInstance;

	FString SetTimeToString(float Time);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

};
