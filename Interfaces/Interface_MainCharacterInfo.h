/*
* Header "Interface_MainCharacterInfo"
* Created by Yang
* Last Modified by Yang
* Description:
* Interface - character & several objects.
*/

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_MainCharacterInfo.generated.h"

UINTERFACE(MinimalAPI)
class UInterface_MainCharacterInfo : public UInterface
{
	GENERATED_BODY()
};

class CAP2_API IInterface_MainCharacterInfo
{
	GENERATED_BODY()

public:
	UFUNCTION()
		virtual bool IsMainCharacter() = 0;
	UFUNCTION()
		virtual void GrapRope(AMaster_Rope* Rope, float HangingLocZ) = 0;
	UFUNCTION()
		virtual bool GetIsPressLMB() = 0;
	UFUNCTION()
		virtual bool GetIsHangingRope() = 0;
};
