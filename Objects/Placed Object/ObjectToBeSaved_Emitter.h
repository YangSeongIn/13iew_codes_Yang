/*
* Header "ObjectToBeSaved_Emitter"
* Created by Yang
* Last Modified by Yang
* Description:
* Emitter data to save
*/

#pragma once

#include "CoreMinimal.h"
#include "ObjectToBeSaved.h"
#include "ObjectToBeSaved_Emitter.generated.h"

/**
 * 
 */
UCLASS()
class CAP2_API AObjectToBeSaved_Emitter : public AObjectToBeSaved
{
	GENERATED_BODY()
	
public:
	AObjectToBeSaved_Emitter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UParticleSystemComponent* Emitter_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UParticleSystemComponent* Emitter_2;

	UPROPERTY(EditAnywhere, Category = "ObjectType")
		bool bSaveVisibility_Emitter;

protected:
	virtual void BeginPlay() override;
	virtual void Save() override;
	virtual void Reset() override;
};
