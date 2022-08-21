#include "Master_InteractableObject.h"
#include "HighlightableComponent.h"

AMaster_InteractableObject::AMaster_InteractableObject()
{
	PrimaryActorTick.bCanEverTick = true;

	_RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = _RootComponent;

	HightlightableComponent = CreateDefaultSubobject<UHighlightableComponent>(TEXT("HightlightableComponent"));
}
