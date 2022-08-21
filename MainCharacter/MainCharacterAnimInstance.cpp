// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterAnimInstance.h"
#include "MainCharacter.h"
#include "Attention.h"

UMainCharacterAnimInstance::UMainCharacterAnimInstance()
{
	Speed = 0.0f; 
}

void UMainCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	OwningActor = TryGetPawnOwner();
	if (OwningActor)
	{
		if (MainCharacter)
		{
			Speed = OwningActor->GetVelocity().Size();
			HangingRopeSpeed = FVector::DotProduct(OwningActor->GetVelocity(), OwningActor->GetActorForwardVector());
			bIsFalling = MainCharacter->GetIsFalling();
			bIsCrouching = MainCharacter->GetIsCrouching();
			bIsRopeHanging = MainCharacter->GetIsRopeHanging();
			bIsAttachedAtMovableObj = MainCharacter->GetIsAttachedAtMovableObj();
			PushPullDir = MainCharacter->GetPushPullDir();
			IKLeftHandEffector = MainCharacter->GetMovableLeftArmIKInfo().Get<0>();
			bIsMovableLeftHandHitWall = MainCharacter->GetMovableLeftArmIKInfo().Get<1>();
			IKRightHandEffector = MainCharacter->GetMovableRightArmIKInfo().Get<0>();
			bIsMovableRightHandHitWall = MainCharacter->GetMovableRightArmIKInfo().Get<1>();
			bIsOnStaticRope = MainCharacter->GetIsStaticRope();
			bCanPushPullDegree = MainCharacter->CanPushOrPullDegree();
			bIsGrabbingThrowableObject = MainCharacter->GetIsGrabbingThrowableObject();
			bIsOnPole = MainCharacter->GetIsOnPole();
			XAxisValue = MainCharacter->GetXAxisValue();
			YAxisValue = MainCharacter->GetYAxisValue();
			bIsFlashMode = MainCharacter->HasFlashLight();
			MainCharacter->CharacterMovementComponent->MovementMode == EMovementMode::MOVE_Swimming ? bIsSwimming = true : bIsSwimming = false;
			bIsWalingOnSwimming = MainCharacter->bIsWalkingOnSwimming;
			//FootPlacement();
			// mj
			bIsOnLadder = MainCharacter->GetIsOnLadder();
			bIsClimbing = MainCharacter->GetIsClimbing();
			bIsSideClimbing = MainCharacter->GetIsSideClimbing();
			LadderSpeed = MainCharacter->GetAxisClimbing();

			bIsGrabPulley = MainCharacter->GetIsGrabbingPulley();

			bIsAttachedAtPullBoard = MainCharacter->bIsAttachedAtPullBoard;

			bIsRunningJumping = MainCharacter->GetIsRunningJumping();
		}
		else
		{
			MainCharacter = Cast<AMainCharacter>(OwningActor);
		}
	}
}

void UMainCharacterAnimInstance::AttentionOn(class AAttention* Attention)
{
	LookAtPos = Attention->GetActorLocation();
	LookAtAlpha = 1.0f;
	bIsLookAt = true;
}

void UMainCharacterAnimInstance::AttentionOff()
{
	bIsLookAt = false;
}

void UMainCharacterAnimInstance::FootPlacement()
{
	if (MainCharacter)
	{
		//if (!bIsFalling)
		//{
			TTuple<bool, float> HitInfo1 = Dis(FName("foot_r"));
			TTuple<bool, float> HitInfo2 = Dis(FName("foot_l"));
			float SelectedDistance;

			if (HitInfo1.Get<0>() || HitInfo2.Get<0>())
			{
				HitInfo1.Get<1>() >= HitInfo2.Get<1>() ? SelectedDistance = HitInfo1.Get<1>() : SelectedDistance = HitInfo2.Get<1>();
				float Target = (SelectedDistance - MainCharacter->GetCapsuleComponentNormalHeight()) * -1;

				Displacement = UKismetMathLibrary::FInterpTo(Displacement, Target, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), IKInterpSpeed);

				TTuple<bool, float, FVector> OutHitInfo1 = LineTrace(FName("foot_r"));
				FRotator Rot1 = UKismetMathLibrary::MakeRotator(FMath::Atan2(OutHitInfo1.Get<2>().Y, OutHitInfo1.Get<2>().Z), -1 * FMath::Atan2(OutHitInfo1.Get<2>().X, OutHitInfo1.Get<2>().Z), 0.0f);
				RRot = UKismetMathLibrary::RInterpTo(RRot, Rot1, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), IKInterpSpeed);
				float Target1 = (OutHitInfo1.Get<1>() - 100.0f) / (-45);
				RIK = UKismetMathLibrary::FInterpTo(RIK, Target1, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), IKInterpSpeed);
				TTuple<bool, float, FVector> OutHitInfo2 = LineTrace(FName("foot_l"));
				FRotator Rot2 = UKismetMathLibrary::MakeRotator(FMath::Atan2(OutHitInfo2.Get<2>().Y, OutHitInfo2.Get<2>().Z), -1 * FMath::Atan2(OutHitInfo2.Get<2>().X, OutHitInfo2.Get<2>().Z), 0.0f);
				LRot = UKismetMathLibrary::RInterpTo(LRot, Rot2, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), IKInterpSpeed);
				float Target2 = (OutHitInfo2.Get<1>() - 100.0f) / (-45);
				LIK = UKismetMathLibrary::FInterpTo(LIK, Target2, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), IKInterpSpeed);
			}
		//}
	}
}

TTuple<bool, float> UMainCharacterAnimInstance::Dis(FName SocketName)
{
	FVector StartLocA = MainCharacter->GetMesh()->GetComponentLocation() + FVector(0.0f, 0.0f, MainCharacter->GetCapsuleComponentNormalHeight());
	FVector StartLocB = FVector(MainCharacter->GetMesh()->GetSocketLocation(SocketName).X, MainCharacter->GetMesh()->GetSocketLocation(SocketName).Y, 0.0f);
	FVector StartLoc = UKismetMathLibrary::MakeVector(StartLocB.X, StartLocB.Y, StartLocA.Z);
	FVector EndLoc = StartLoc - FVector(0.0f, 0.0f, 100.0f); // 80
	FHitResult OutHit;
	TArray<AActor*> IgnoreActors;

	bool IsHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartLoc, EndLoc, ETraceTypeQuery::TraceTypeQuery1, false, IgnoreActors, EDrawDebugTrace::None, OutHit, true);

	return MakeTuple(IsHit, OutHit.Distance);
}

TTuple<bool, float, FVector> UMainCharacterAnimInstance::LineTrace(FName SocketName)
{
	FVector StartLocA = MainCharacter->GetMesh()->GetSocketLocation(SocketName);
	FVector StartLocB = MainCharacter->GetMesh()->GetSocketLocation(FName("root"));
	FVector StartLoc = UKismetMathLibrary::MakeVector(StartLocA.X, StartLocA.Y, StartLocB.Z) - FVector(0.0f, 0.0f, -100.0f);
	FVector EndLoc = UKismetMathLibrary::MakeVector(StartLocA.X, StartLocA.Y, StartLocB.Z) - FVector(0.0f, 0.0f, 100.0f);
	FHitResult OutHit;
	TArray<AActor*> IgnoreActors;
	bool IsHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartLoc, EndLoc, ETraceTypeQuery::TraceTypeQuery1, false, IgnoreActors, EDrawDebugTrace::ForOneFrame, OutHit, true);
	if (IsHit)
	{
		return MakeTuple(OutHit.IsValidBlockingHit(), OutHit.Distance, OutHit.Normal);
	}
	else
	{
		return MakeTuple(OutHit.IsValidBlockingHit(), 999.0f, FVector(0.0f));
	}
}

