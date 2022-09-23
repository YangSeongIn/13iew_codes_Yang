/*
* Header "Rope_NormalRope"
* Created by Yang
* Last Modified by Yang
* Description:
* Child of Master_Rope. Normal Rope.
*/

#pragma once

#include "CoreMinimal.h"
#include "Master_Rope.h"

#include "Rope_NormalRope.generated.h"

UCLASS()
class CAP2_API ARope_NormalRope : public AMaster_Rope
{
	GENERATED_BODY()
public:

public:
	ARope_NormalRope();

	void AddSwingForce(FVector SwingVector);
};
