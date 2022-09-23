#include "GaugeWidget.h"
#include "Kismet/KismetMathLibrary.h"

void UGaugeWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);
	if (bIsInteracting)
	{
		GaugeImage->GetDynamicMaterial()->SetScalarParameterValue(FName("Decimal"), LerpValveGauge);
	}
}


void UGaugeWidget::SetValveGauge(float Gauge, bool b)
{
	LerpValveGauge = Gauge / 100;
	bIsInteracting = b;
}
