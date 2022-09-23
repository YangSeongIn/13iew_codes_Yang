/*
* Header "MainCharacterAnimInstance"
* Created by Yang
* Last Modified by Yang
* Description:
* Character Animation Instance.
*/

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Vector.h"
#include "Interface_Attention.h"

#include "MainCharacterAnimInstance.generated.h"

class AMainCharacter;

UCLASS(transient, Blueprintable, hideCategories = AnimInstance, BlueprintType)
class CAP2_API UMainCharacterAnimInstance : public UAnimInstance, public IInterface_Attention
{
	GENERATED_BODY()

private:
	
	AActor* OwningActor;

	
	float IKInterpSpeed = 15.0f;
	

public:
	UMainCharacterAnimInstance();
	UPROPERTY(BlueprintReadWrite)
		class AMainCharacter* MainCharacter;
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
		float Speed;
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
		float HangingRopeSpeed;
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
		float PushPullDir;
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
		float XAxisValue;
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
		float YAxisValue;
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
		bool bIsCrouching;
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
		bool bIsFalling;
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
		bool bIsRopeHanging;
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
		bool bIsAttachedAtMovableObj;
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
		bool bIsMovableLeftHandHitWall;
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
		bool bIsMovableRightHandHitWall;
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
		bool bIsOnStaticRope;
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
		bool bCanPushPullDegree;
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
		bool bIsGrabbingThrowableObject;
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
		bool bIsOnPole;
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
		bool bIsFlashMode;
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
		bool bIsSwimming;
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
		bool bIsWalingOnSwimming;
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
		bool bIsRunningJumping;
	
	// Attention
	UPROPERTY(BlueprintReadWrite, Category = "HeadTrace")
		bool bIsLookAt;
	UPROPERTY(BlueprintReadWrite, Category = "HeadTrace")
		FVector LookAtPos;
	UPROPERTY(BlueprintReadWrite, Category = "HeadTrace")
		float LookAtAlpha = 1.0f;
	virtual void AttentionOn(class AAttention* Attention);
	virtual void AttentionOff();

	// mj
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
		bool bIsOnLadder;
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
		bool bIsClimbing;
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
		bool bIsSideClimbing;
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
		float LadderSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		bool bIsGrabPulley;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		bool bIsAttachedAtPullBoard;

	void FootPlacement();
	TTuple<bool, float> Dis(FName SocketName);
	TTuple<bool, float, FVector> LineTrace(FName SocketName);

	UPROPERTY(BlueprintReadWrite, Category = "FootIK")
		FRotator RRot = FRotator(0.0f, 0.0f, 0.0f);
	UPROPERTY(BlueprintReadWrite, Category = "FootIK")
		FRotator LRot = FRotator(0.0f, 0.0f, 0.0f);
	UPROPERTY(BlueprintReadWrite, Category = "FootIK")
		float RIK = 0.0f;
	UPROPERTY(BlueprintReadWrite, Category = "FootIK")
		float LIK = 0.0f;
	UPROPERTY(BlueprintReadWrite, Category = "FootIK")
		float Displacement;

	UPROPERTY(BlueprintReadWrite, Category = "FootIK")
		FVector IKLeftHandEffector;
	UPROPERTY(BlueprintReadWrite, Category = "FootIK")
		FVector IKRightHandEffector;
	UPROPERTY(BlueprintReadWrite, Category = "FootIK")
		float IKHipOffset;

	UAnimNotify* FootStepNotify;

protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
