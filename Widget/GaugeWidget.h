// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"

#include "GaugeWidget.generated.h"

/**
 * 
 */
UCLASS()
class CAP2_API UGaugeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
		UImage* GaugeImage;
	float LerpValveGauge;
	bool bIsInteracting;
	void SetValveGauge(float Gauge, bool b);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;
};
