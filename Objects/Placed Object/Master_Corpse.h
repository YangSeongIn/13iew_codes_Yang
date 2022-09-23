/*
* Header "Master_Corpse"
* Created by Yang
* Last Modified by Yang
* Description:
* Generating by GeneratorUsingTimer.h.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Master_Corpse.generated.h"

UCLASS()
class CAP2_API AMaster_Corpse : public AActor
{
	GENERATED_BODY()

public:
	AMaster_Corpse();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* StaticMesh;
};