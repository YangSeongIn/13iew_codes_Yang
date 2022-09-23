/*
* Header "ObjectToBeSaved"
* Created by Yang
* Last Modified by Yang
* Description:
* object to save data
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectToBeSaved.generated.h"

UCLASS()
class CAP2_API AObjectToBeSaved : public AActor
{
	GENERATED_BODY()
	
public:	
	AObjectToBeSaved();

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMesh;

	FTransform InitTransform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTransform ChangedTransform;

	UPROPERTY(BlueprintReadWrite)
		bool bIsTransform;

	UPROPERTY(EditAnywhere, Category = "ObjectType")
		bool bSaveTransform;
	UPROPERTY(EditAnywhere, Category = "ObjectType")
		bool bSaveVisibility;

	UPROPERTY()
		class UMyGameInstance* GameInstance;
	UFUNCTION()
		virtual void Reset();
	UFUNCTION()
		virtual void Save();

protected:
	virtual void BeginPlay() override;
};
