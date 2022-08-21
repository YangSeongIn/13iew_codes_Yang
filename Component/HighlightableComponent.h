#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HighlightableComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CAP2_API UHighlightableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHighlightableComponent();

	bool bIsOnFocus;
	int8 DoN = 0;

	class AMainCharacter* MainCharacter;

	UStaticMeshComponent* StaticMesh;
	class UCableComponent* Cable;

	UFUNCTION()
		void TurnOnHighlight();
	
	FLatentActionInfo LAI;

	UFUNCTION()
		void DelayFunc();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
