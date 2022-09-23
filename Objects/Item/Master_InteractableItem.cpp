#include "Master_InteractableItem.h"
#include "HighlightableComponent.h"

AMaster_InteractableItem::AMaster_InteractableItem()
{
	PrimaryActorTick.bCanEverTick = false;

	_RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = _RootComponent;

	HightlightableComponent = CreateDefaultSubobject<UHighlightableComponent>(TEXT("HightlightableComponent"));
}
