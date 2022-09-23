/*
* Header "Projector"
* Created by Yang
* Last Modified by Yang
* Description:
* A gimmick that plays a player when it receives light. It is located in the projector room on the 1st floor of Level 1.
*/


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "MediaAssets/Public/MediaPlayer.h"
#include "MediaAssets/Public/MediaSource.h"
#include "Components/SpotLightComponent.h"

#include "Projector.generated.h"

UCLASS()
class CAP2_API AProjector : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjector();

	bool bDoOncePlay = true;

	class UMyGameInstance* GameInstance;

	UStaticMeshComponent* SM_Screen;
	UMaterialInterface* DefaultMaterial;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere)
		AActor* Screen;
	UPROPERTY(EditAnywhere)
		UBoxComponent* BoxComponent;
	UPROPERTY(EditAnywhere)
		UMediaPlayer* MP;
	UPROPERTY(EditAnyWhere)
		UMediaSource* MS;
	UPROPERTY(EditAnyWhere)
		USpotLightComponent* SpotLight_B;
	UPROPERTY(EditAnyWhere)
		USpotLightComponent* SpotLight_W;

	UPROPERTY(EditAnyWhere)
		UMaterialInterface* MPMaterial;

	class  AWayPoints* WayPointsCast;
public:	
	void ProjectVideo();

	UFUNCTION()
		void Reset();

protected:
	virtual void BeginPlay() override;
};
