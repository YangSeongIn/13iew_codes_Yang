#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_Attention.generated.h"

UINTERFACE(MinimalAPI)
class UInterface_Attention : public UInterface
{
	GENERATED_BODY()
};

class CAP2_API IInterface_Attention
{
	GENERATED_BODY()

public:
	virtual void AttentionOn(class AAttention* Attention) = 0;
	virtual void AttentionOff() = 0;
};
