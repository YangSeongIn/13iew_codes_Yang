#include "KeyGuideWidget.h"
#include "Components/TextBlock.h"

void UKeyGuideWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayAnimation(Fade);
}

void UKeyGuideWidget::SetKeyGuideText(FString ObjectName)
{
	FString Str = CheckWhatKeyGuide(ObjectName);
	KeyGuideText->SetText(FText::FromString(Str));
}

FString UKeyGuideWidget::CheckWhatKeyGuide(FString ObjectName)
{
	FString StringResult;
	if (ObjectName == "BasicMovement")
	{
		StringResult = "W, A, S, D : Move\nSpace Bar : Jump\nLeft Ctrl : Crouch\nLeft Shift : Sprint";
	}
	else if (ObjectName == "Sliding")
	{
		StringResult = "Left Ctrl during Sprinting : Sliding";
	}
	else if (ObjectName == "PullBoard")
	{
		StringResult = "Left Mouse Button : Grab\nA : Pull Board";
	}
	else if (ObjectName == "Ledge")
	{
		StringResult = "Left Mouse Button : Grab Ledge\nW : Climb\nA, D : Move On Ledge";
	}
	else if (ObjectName == "Pulley")
	{
		StringResult = "Left Mouse Button : Grab Pulley";
	}
	else if (ObjectName == "Rope")
	{
		StringResult = "Left Mouse Button : Grab Rope\nW, A, S, D : Swing";
	}
	else if (ObjectName == "Ladder")
	{
		StringResult = "Left Mouse Button : Grab Ladder\nW, S : Up & Down";
	}
	else if (ObjectName == "PushPull")
	{
		StringResult = "Left Mouse Button : Grab Object\nW, A, S, D : Pull or Push";
	}
	else if (ObjectName == "Throwable")
	{
		StringResult = "Left Mouse Button : Pick Up\nSpace Bar : Throw";
	}
	else if (ObjectName == "Pole")
	{
		StringResult = "Left Mouse Button : Grab Pole\nW, S : Up & Down";
	}
	else if (ObjectName == "FlashLight")
	{
		StringResult = "F : Flash Light";
	}
	else if (ObjectName == "KeyExplanation")
	{
		StringResult = "P : Open Key Guide";
	}
	else if (ObjectName == "Door")
	{
		StringResult = "E : Open the Door When 1st Person View";
	}
	else if (ObjectName == "Hide")
	{
		StringResult = "E : Hide in Object";
	}
	else if (ObjectName == "FindFlashLight")
	{
		StringResult = "Find Flash Light";
	}
	else
	{
		StringResult = "암것도 아님";
	}

	return StringResult;
}