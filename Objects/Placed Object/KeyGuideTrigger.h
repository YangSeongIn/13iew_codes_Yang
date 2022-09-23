/*
* Header "KeyGuideTrigger"
* Created by Yang
* Last Modified by Yang
* Description:
* It is a trigger box that notifies the user of the key.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"

#include "KeyGuideTrigger.generated.h"

UCLASS()
class CAP2_API AKeyGuideTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	AKeyGuideTrigger();

	void ShowKeyGuideOnWidget();

	UPROPERTY(EditAnywhere)
		UBoxComponent* Box;
	UPROPERTY(EditAnywhere)
		FString GimmickName;

	UPROPERTY()
		bool bDoOnce = true;
	UPROPERTY(EditAnywhere)
		float KeepingTime;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> _KeyGuideWidget;
	UPROPERTY()
		class UKeyGuideWidget* KeyGuideWidget;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	virtual void BeginPlay() override;
};
