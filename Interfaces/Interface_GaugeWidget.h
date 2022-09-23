/*
* Header "Interface_GaugeWidget"
* Created by Yang
* Last Modified by Yang
* Description:
* Interface - Widget & Valve
*/

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_GaugeWidget.generated.h"

UINTERFACE(MinimalAPI)
class UInterface_GaugeWidget : public UInterface
{
	GENERATED_BODY()
};

class CAP2_API IInterface_GaugeWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
		virtual float GetValveGauge() = 0;
};
