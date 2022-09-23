#include "MainCharacter.h"
#include "Master_InteractableObject.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "ESCMenuWidget.h"
#include "Master_Movable.h"
#include "MyGameInstance.h"
#include "Master_Rope.h"
#include "Master_InteractableItem.h"
#include "FirstPersonDefaultWidget.h"
#include "Rope_NormalRope.h"
#include "Rope_Handle.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "SaveGameObject.h"
#include "Master_Inspect.h"
#include "Master_Slicable.h"
#include "Master_Throwable.h"
#include "Master_Pole.h"
#include "Components/Image.h"
#include "HighlightableComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "KeyExplanationWidget.h"
#include "MyEnums.h"
#include "InteractableObject_Valve.h"
#include "InteractableObject_Drawer.h"
#include "GameFramework/HUD.h"
#include "SoundManager.h"
#include "CameraUtility.h"   
#include "Pulley.h"
#include "NumberLock.h"
#include "MachineRoomPuzzle.h"
#include "InteractableObject_Hidable.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	TriggerCapsuleComponent = this->GetCapsuleComponent();
	RootComponent = TriggerCapsuleComponent;
	TriggerCapsuleComponent->SetCapsuleHalfHeight(CapsuleDefaultHeight);
	TriggerCapsuleComponent->SetCapsuleRadius(CapsuleDefaultRadius);
	TriggerCapsuleComponent->SetCollisionProfileName(FName("Custom..."));

	SkeletalMesh = GetMesh();
	SkeletalMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -65.0f));
	SkeletalMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera Comp"));
	FirstPersonCameraComponent->SetupAttachment(SkeletalMesh);
	FirstPersonCameraComponent->SetRelativeLocation(FVector(0.0f, -25.0f, 94.0f));
	FirstPersonCameraComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->PostProcessSettings.AutoExposureMaxBrightness = 8.0f;
	FirstPersonCameraComponent->PostProcessSettings.AutoExposureMinBrightness = 8.0f;

	CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->MaxWalkSpeed = DefaultWalkSpeed;
	CharacterMovementComponent->MaxWalkSpeedCrouched = DefaultCrouchSpeed;
	CharacterMovementComponent->bOrientRotationToMovement = true;
	CharacterMovementComponent->RotationRate = FRotator(0, 540, 0);
	CharacterMovementComponent->GetNavAgentPropertiesRef().bCanCrouch = true;
	CharacterMovementComponent->JumpZVelocity = DefaultJumpVelocity;
	CharacterMovementComponent->MaxAcceleration = 600.0f;
	CharacterMovementComponent->bUseSeparateBrakingFriction = true;
	CharacterMovementComponent->BrakingFrictionFactor = 0.2f;
	CharacterMovementComponent->BrakingFriction = 0.06f;
	CharacterMovementComponent->AirControl = 1.0f;
	CharacterMovementComponent->CrouchedHalfHeight = CrouchHeight;
	CharacterMovementComponent->bCanWalkOffLedgesWhenCrouching = true;
	CharacterMovementComponent->MaxSwimSpeed = 150;
	CharacterMovementComponent->BrakingDecelerationSwimming = 100;

	PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcess"));
	PostProcessComponent->SetupAttachment(RootComponent);

	FlashLight_FP = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlashLight_FP"));
	FlashLight_FP->SetupAttachment(FirstPersonCameraComponent);
	FlashLight_FP->SetVisibility(false);
	FlashLight_FP->SetCollisionProfileName("NoCollision");

	SpotLight_FP = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight_FP"));
	SpotLight_FP->SetupAttachment(FlashLight_FP);
	SpotLight_FP->SetVisibility(false);
	SpotLight_FP->Intensity = 20000;
	SpotLight_FP->AttenuationRadius = 3000;
	SpotLight_FP->OuterConeAngle = 40;
	SpotLight_FP->InnerConeAngle = 30;

	FlashLight_SV = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlashLight_SV"));
	FlashLight_SV->SetupAttachment(SkeletalMesh);
	FlashLight_SV->SetVisibility(false);
	FlashLight_SV->SetCollisionProfileName("NoCollision");


	SpotLight_SV = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight_SV"));
	SpotLight_SV->SetupAttachment(FlashLight_SV);
	SpotLight_SV->SetVisibility(false);
	SpotLight_SV->Intensity = 30000;
	SpotLight_SV->AttenuationRadius = 3000;
	SpotLight_SV->OuterConeAngle = 40;
	SpotLight_SV->InnerConeAngle = 30;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	TurnRate = 45.0f;
	LookUpRate = 45.0f;

	SmoothCrouchCurveTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("SmoothCrouchTimeline"));
	SmoothCrouchInterpFunction.BindUFunction(this, FName("SmoothCrouchInterpReturn"));

	static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> M_FPCameraWaterEffect(TEXT("MaterialInstanceConstant'/Game/UsableObjects/Water/Mat_WaterPostProcess_Inst.Mat_WaterPostProcess_Inst'"));
	if (M_FPCameraWaterEffect.Succeeded())
	{
		FPCameraWaterEffect = M_FPCameraWaterEffect.Object;
	}

	SetupStimulus();

}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitialSetting();
	TimelineSetting();
	DOFSetting();
	DelegateSetting();
	SaveLoadSetting();

	GameInstance->LoadGame();
	GameInstance->LoadData();

	FlashLight_SV->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("FlashLightSocket"));
	DefaultWidget->PlayAnimation(DefaultWidget->FadeIn);

}

void AMainCharacter::InitialSetting()
{
	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	PlayerController = GetWorld()->GetFirstPlayerController();

	if (_DefaultWidget)
	{
		DefaultWidget = Cast<UFirstPersonDefaultWidget>(CreateWidget(GetWorld(), _DefaultWidget));
		if (DefaultWidget)
		{
			DefaultWidget->AddToViewport();
		}
	}

	// mj
	bIsOnLadder = false;
	bIsOnLadderTop = false;
	AxisClimbing = 0.0f;
	TimeToEnableGrabLadder = 0.0f;

	bIsOnPulley = false;
	bIsGrabbingPulley = false;
}

void AMainCharacter::TimelineSetting()
{
	if (SmoothCrouchCurveFloat)
	{
		SmoothCrouchCurveTimeline->AddInterpFloat(SmoothCrouchCurveFloat, SmoothCrouchInterpFunction);
		SmoothCrouchCurveTimeline->SetTimelineFinishedFunc(SmoothCrouchTimelineFinish);
		SmoothCrouchCurveTimeline->SetLooping(false);
	}
}

void AMainCharacter::DOFSetting()
{
	GetWorldTimerManager().SetTimer(DOFTimerHandle, this, &AMainCharacter::DOF, 0.1f, true, 0.0f);
	PowerDOF = 1.5f;
}

void AMainCharacter::DelegateSetting()
{
	Delegate_OnMontageNotifyBegin.BindUFunction(this, FName("OnMontageNotifyBegin"));
	SkeletalMesh->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &AMainCharacter::OnMontageEnded);
	SkeletalMesh->GetAnimInstance()->OnPlayMontageNotifyBegin.Add(Delegate_OnMontageNotifyBegin);
	SkeletalMesh->GetAnimInstance()->OnMontageBlendingOut.AddDynamic(this, &AMainCharacter::OnMontageBlendOut);
}

void AMainCharacter::SaveLoadSetting()
{
	if (GameInstance)
	{
		GameInstance->OnSave.AddUFunction(this, FName("Save"));
		GameInstance->OnReset.AddUFunction(this, FName("Reset"));
	}
}

void AMainCharacter::SmoothCrouchInterpReturn(float Value)
{
	TriggerCapsuleComponent->SetCapsuleHalfHeight(FMath::Lerp(CapsuleDefaultHeight, CrouchHeight, Value));
	FirstPersonCameraComponent->SetRelativeLocation(FVector(0.0f, -25.0f, (FMath::Lerp(110.0f, 90.0f, Value))));
}

void AMainCharacter::CrawlingInterpReturn(float Value)
{
	CrawlingSpeed = Value;
}

void AMainCharacter::FallingDownInterpReturn(float Value)
{
	FallingDownSpeed = Value;
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Yang

	bIsDefaultState = bIsControllable && !bIsGrabbingThrowable && !bIsOnPole && !bIsAttachedAtMovable && !bIsSliding && !bIsHangingRope && !bIsOnLedge && !bIsCrouching && !bIsOnLadder && !bIsOnPulley;
	bIsDefaultStateNotIncludingCrouching = bIsControllable && !bIsGrabbingThrowable && !bIsOnPole && !bIsAttachedAtMovable && !bIsSliding && !bIsHangingRope && !bIsOnLedge && !bIsOnLadder && !bIsGrabbingPulley;

	CharacterMaxWalkSpeedManager();
	FallingManager();
	CrouchManager();
	LineTraceHud();
	PushPullManager();
	ThrowManager();
	DefaultWidgetManager();
	MoveLedgeManager();
	SwimmingManager();

	// Kang
	// Ledge
	LedgeManager();

	// mj
	ClimbLadder(DeltaTime);
	ClimbLadderSide(DeltaTime);
	EndLadderTop();
	CountTimeToEnableGrabLadder(DeltaTime);
	//CheckLadderTop();
	//ClimbByStep(DeltaTime);
	KeepGrabbingPulley();
	GrabPulley(bIsPressLMB);
	RollLockWheel(DeltaTime);

	// Park
	PullBoardManager();
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AMainCharacter::LookX);
	PlayerInputComponent->BindAxis("LookUp", this, &AMainCharacter::LookY);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMainCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMainCharacter::StopSprint);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMainCharacter::StopJump);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMainCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMainCharacter::StopCrouch);
	PlayerInputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &AMainCharacter::LeftMouseButtonPress);
	PlayerInputComponent->BindAction("LeftMouseButton", IE_Released, this, &AMainCharacter::LeftMouseButtonRelease);
	PlayerInputComponent->BindAction("SwitchCamera", IE_Pressed, this, &AMainCharacter::SwitchCamera);
	PlayerInputComponent->BindAction("ClimbUp", IE_Pressed, this, &AMainCharacter::ClimbUp); //Ledge
	PlayerInputComponent->BindAction("ESCMenu", IE_Pressed, this, &AMainCharacter::ESCMenu).bExecuteWhenPaused = true;
	PlayerInputComponent->BindAction("FlashLight", IE_Pressed, this, &AMainCharacter::FlashLight);
	PlayerInputComponent->BindAction("LedgeMove", IE_Pressed, this, &AMainCharacter::LedgeMove);
	PlayerInputComponent->BindAction("OpenKeyExplanationWidget", IE_Pressed, this, &AMainCharacter::OpenKeyExplanationWidget).bExecuteWhenPaused = true;
	PlayerInputComponent->BindAction("CheckEdgeSlip", IE_Released, this, &AMainCharacter::CheckEdgeSlip);

	PlayerInputComponent->BindAction("TestDie", IE_Pressed, this, &AMainCharacter::TestDie);
	// Interaction
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMainCharacter::Interact);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &AMainCharacter::StopInteract);
	// CameraShake



	//mj
	//PlayerInputComponent->BindAction("InputKeyboardF", IE_Pressed, this, &AMainCharacter::InputKeyboardF);
}

void AMainCharacter::TestDie()
{
	Die();
}

void AMainCharacter::LookX(float AxisValue)
{
	float _AxisValue = (MouseSensitivityValue + MouseSensitivityWeight) * AxisValue;
	if (bIsControllable && bIsFP && !bIsOnLedge)
	{
		AddControllerYawInput(_AxisValue * GetWorld()->GetDeltaSeconds() * TurnRate);
	}
}

void AMainCharacter::LookY(float AxisValue)
{
	float _AxisValue = (MouseSensitivityValue + MouseSensitivityWeight)* AxisValue;
	if (bIsControllable && bIsFP && !bIsOnLedge)
	{
		AddControllerPitchInput(_AxisValue * GetWorld()->GetDeltaSeconds() * LookUpRate);
	}
}

void AMainCharacter::MoveForward(float AxisValue)
{
	if (bIsControllable && Controller && !bIsOnLedge)
	{
		XAxisValue = AxisValue;
		if (AxisValue != 0.0f)
		{
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			if (bIsHangingRope && !bIsStaticRope && _Rope && _Rope->GetClass()->IsChildOf(ARope_NormalRope::StaticClass()))   // Rope
			{
				ARope_NormalRope* Rope = Cast<ARope_NormalRope>(_Rope);
				Rope->AddSwingForce(Direction * AxisValue);
			}

			else if (bIsAttachedAtMovable)
			{

			}
			else if (bIsOnPole)
			{
				TriggerCapsuleComponent->AddWorldOffset(FVector(0.0f, 0.0f, AxisValue / 1.5), true);
			}
			else if (CharacterMovementComponent->MovementMode == EMovementMode::MOVE_Swimming)
			{
				if (bIsFP)
				{
					AddMovementInput(FirstPersonCameraComponent->GetForwardVector(), AxisValue);
				}
				else
				{
					AddMovementInput(Direction, AxisValue);
				}
			}
			//mj
			else if (bIsOnLadder)
			{
				bool bIsOnLadTop = false;
				bool bIsOnLadBottom = false;
				ALadder* Lad = Cast<ALadder>(OwnLadder);
				if (Lad)
				{
					bIsOnLadTop = Lad->GetIsOnTop();
					bIsOnLadBottom = Lad->GetIsOnBottom();
				}

				if (bIsOnLadTop && AxisValue > 0 || bIsOnLadBottom && AxisValue < 0)
				{
					AxisClimbing = 0;
				}
				else
				{
					AxisClimbing = AxisValue;
				}
			}
			else if (bIsOnPulley)
			{

			}
			else if (bIsAttachedAtPullBoard)
			{

			}
			else if (bIsDefaultStateNotIncludingCrouching || bIsGrabbingThrowable)	// Normal 
			{
				if (bIsFP)
				{
					if (bIsSprinting)
					{
						PlayerController->PlayerCameraManager->StartCameraShake(CameraShake_Sprint, 1.0f);
					}
					else if (!bIsHangingRope)
					{
						PlayerController->PlayerCameraManager->StartCameraShake(CameraShake_Walk, 1.0f);
					}
				}
				AddMovementInput(Direction, AxisValue);
			}	
		}
		//mj
		else if (bIsOnLadder)
		{
			AxisClimbing = 0.0f;
			//CharacterMovementComponent->StopMovementImmediately();
		}
		//--

	}

}

void AMainCharacter::MoveRight(float AxisValue)
{
	if (bIsControllable && Controller && !bIsOnLedge)
	{
		//mj
		if (bIsOnLadder) {
			if (!Cast<ALadder>(OwnLadder)->GetIsEnableSideMove())
				return;
		}
		//---

		YAxisValue = AxisValue;
		if (AxisValue != 0.0f)
		{
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			if (bIsHangingRope && !bIsStaticRope && _Rope && _Rope->GetClass()->IsChildOf(ARope_NormalRope::StaticClass()))   // Rope
			{
				ARope_NormalRope* Rope = Cast<ARope_NormalRope>(_Rope);
				Rope->AddSwingForce(Direction * AxisValue);
			}
			else if (bIsAttachedAtMovable)
			{

			}
			else if (bIsOnPole)
			{

			}
			else if (CharacterMovementComponent->MovementMode == EMovementMode::MOVE_Swimming)
			{
				if (bIsFP)
				{
					AddMovementInput(FirstPersonCameraComponent->GetRightVector(), AxisValue);
				}
				else
				{
					AddMovementInput(Direction, AxisValue);
				}
			}
			//mj
			else if (bIsOnLadder)
			{
				bool bIsOnLadTop = false;
				bool bIsOnLadBottom = false;
				ALadder* Lad = Cast<ALadder>(OwnLadder);
				if (Lad)
				{
					bIsOnLadTop = Lad->GetIsOnTop();
					bIsOnLadBottom = Lad->GetIsOnBottom();
				}

				if (bIsOnLadTop || bIsOnLadBottom)
				{
					AxisSideClimbing = 0;	//?
				}
				else
				{
					AxisSideClimbing = AxisValue;
				}
			}
			else if (bIsOnPulley)
			{
				// prevent rotation
			}
			//--
			else if (bIsAttachedAtPullBoard)
			{

			}
			else if (bIsDefaultStateNotIncludingCrouching || bIsGrabbingThrowable)   // Normal
			{
				if (bIsFP)
				{
					if (bIsSprinting)
					{
						PlayerController->PlayerCameraManager->StartCameraShake(CameraShake_Sprint, 1.0f);
					}
					else if (!bIsHangingRope)
					{
						PlayerController->PlayerCameraManager->StartCameraShake(CameraShake_Walk, 1.0f);
					}
				}
				AddMovementInput(Direction, AxisValue);
			}
		}
		//mj
		else if (bIsOnLadder)
		{
			AxisSideClimbing = 0.0f;
		}
	}
}

void AMainCharacter::StartSprint()
{
	bIsPressingSprintKey = true;
	if (bIsControllable)
	{
		bIsSprinting = true;
	}
}

void AMainCharacter::StopSprint()
{
	bIsPressingSprintKey = false;
	if (bIsControllable)
	{
		bIsSprinting = false;
	}
}

void AMainCharacter::StartJump()
{
	bPressJumpKey = true;
	// Throw
	if (!bIsPickingUp && bIsGrabbingThrowable)
	{
		bIsThrowKeyDown = true;
		ThrowGauge = 0.5f;
		if (M_ReadyToThrow)
		{
			PlayAnimMontage(M_ReadyToThrow, 0.5f);
			bIsControllable = false;
		}
	}
	// Pole Jump
	else if (bIsOnPole)
	{
		CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);
		bIsOnPole = false;
		_Pole->StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		_Pole = nullptr;
		Jump();
	}
	// mj
	// Ladder Jump
	else if (bIsOnLadder)
	{
		CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);
		bIsOnLadder = false;

		bPressedJump = true;
		Jump();
	}
	// Default
	else if (bIsDefaultState)
	{
		bPressedJump = true;
		if(!bIsFalling)
			GetVelocity().Size() >= 250 ? bIsRunningJumping = true : bIsRunningJumping = false;
		Jump();
	}
}

void AMainCharacter::StopJump()
{
	// Throw
	if (!bIsPickingUp && bIsGrabbingThrowable)
	{
		bIsThrowKeyDown = false;
		bIsGrabbingThrowable = false;
		ThrowableObject->Throw(this, ThrowGauge);
		PlayAnimMontage(M_Throw);
		bIsControllable = false;
	}
	// Pole
	else if (bIsOnPole)
	{
		StopJumping();
	}
	// Default
	else if (bIsDefaultState)
	{
		bPressedJump = false;
		StopJumping();
	}
}

void AMainCharacter::StartCrouch()
{
	bIsPressingCrouchingKey = true;
	// Sliding
	if (UKismetMathLibrary::NearlyEqual_FloatFloat(GetVelocity().Size(), DefaultSprintSpeed, 1.0f) && bIsSprinting && !bIsFP && bIsControllable && !bIsGrabbingThrowable)
	{
		bIsControllable = false;
		bIsSliding = true;
		CharacterMovementComponent->BrakingDecelerationWalking = 200.0f;

		if (M_Slide)
		{
			TurnOffFlashLight();
			PlayAnimMontage(M_Slide);
		}
	}
	// Normal Crouch
	else if (bIsDefaultState)
	{

		if (bIsControllable && !bIsFalling)
		{
			bIsCrouching = true;
			if (!bIsFP)
			{
				Crouch();
			}
			else
			{
				SmoothCrouchCurveTimeline->Play();
			}
		}
	}
}

void AMainCharacter::StopCrouch()
{
	bIsPressingCrouchingKey = false;
	if (bIsControllable)
	{
		bIsCrouching = false;
		if (!bIsFP)
		{
			UnCrouch();
		}
	}
}

void AMainCharacter::LineTraceHud()
{
	if (bIsFP)
	{
		FVector StartLoc = FirstPersonCameraComponent->GetComponentLocation();
		FVector EndLoc = StartLoc + FirstPersonCameraComponent->GetForwardVector() * 200.0f;
		TArray<AActor*> ToIgnore;
		FHitResult OutHit;

		bool bIsOutHit = UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			StartLoc,
			EndLoc,
			ETraceTypeQuery::TraceTypeQuery6,
			false,
			ToIgnore,
			EDrawDebugTrace::None,
			OutHit,
			true
		);
		bIsOnFocusHighlightable = bIsOutHit;
		if (bIsOutHit)
		{
			UHighlightableComponent* HLC = Cast<UHighlightableComponent>(OutHit.Actor->GetComponentByClass(UHighlightableComponent::StaticClass()));

			if (HLC)
			{
				HLC->TurnOnHighlight();
			}

			// Door Focus Icon
			InterfaceWithWidget = Cast<IInterface_InteractWithWidget>(OutHit.Actor);
			if (InterfaceWithWidget)
			{
				InterfaceWithWidget->OnFocus();
			}
		}
	}
}

void AMainCharacter::LineTraceLMB()
{
	if (bIsDefaultState)
	{
		PressLMBForMovable();
		PressLMBForThrow();
		PressLMBForClimbingPole();
		PressLMBForPullBoard();
	}
}

void AMainCharacter::LineTraceLMBReleased()
{
	ReleaseLMBForMovable();
	ReleaseLMBForThrow();
	ReleaseLMBClimbingPole();
	ReleaseLMBForPullBoard();
}

// if FirstPerson
void AMainCharacter::Interact()
{
	FVector Start;
	FVector End;
	FHitResult OutHit;
	float InteractDistance = 200.0f;

	if (bIsInspectingLock)
	{
		ToggleLockInspector(false);
	}
	else if (bIsInspectingPuzzle) {
		TogglePuzzleInspector(false);
	}
	else if (bIsFP)
	{
		Start = FirstPersonCameraComponent->GetComponentLocation();
		End = Start + FirstPersonCameraComponent->GetForwardVector() * InteractDistance;

		TArray<AActor*> IgnoreActors;
		bool bIsInteracting = UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			Start,
			End,
			ETraceTypeQuery::TraceTypeQuery1,
			false,
			IgnoreActors,
			EDrawDebugTrace::None,
			OutHit,
			true
		);

		if (bIsInteracting)
		{
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Blue, TEXT("Press E Key"));
			AMaster_InteractableObject* Obj = Cast<AMaster_InteractableObject>(OutHit.Actor);
			AMaster_InteractableItem* Item = Cast<AMaster_InteractableItem>(OutHit.Actor);
			AMaster_Inspect* InspectItem = Cast<AMaster_Inspect>(OutHit.Actor);
			ANumberLock* Lock = Cast<ANumberLock>(OutHit.Actor);
			AMachineRoomPuzzle* Puzzle = Cast<AMachineRoomPuzzle>(OutHit.Actor);
			
			if (Obj)
			{
				if (InteractableObject == nullptr)
				{
					InteractableObject = Obj;
					InteractableObject->Interact();
				}
			}
			else if (Item)
			{
				Item->Interact();
			}
			else if (InspectItem && bIsFP && !bIsFalling)
			{
				CharacterMovementComponent->StopMovementImmediately();
				bIsControllable = false;
				InspectItem->Inspect(this);
			}
			else if (Lock)
			{
				{
					NumberLock = Lock;
					ToggleLockInspector(true);
				}
			}
			else if (Puzzle) {
				MPuzzle = Puzzle;
				TogglePuzzleInspector(true);
			}
		}
		else
		{
			// Hide Out
			EscapeFromHidableObject();
		}
	}
}

void AMainCharacter::StopInteract()
{
	AInteractableObject_Valve* Valve = Cast<AInteractableObject_Valve>(InteractableObject);
	if (Valve)
	{
		Valve->StopInteract();
	}
	InteractableObject = nullptr;
}

void AMainCharacter::LeftMouseButtonPress()
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Blue, TEXT("Press Left Mouse Button"));
	bIsPressLMB = true;
	LineTraceLMB();


	// mj - ladder
	GrabLadder(true);
	// mj - pulley
	//GrabPulley(true);
	// mj - numberlock
	ClickLockWheel(true);
	ClickPuzzleButton(true);

}

void AMainCharacter::LeftMouseButtonRelease()
{
	bIsPressLMB = false;
	LineTraceLMBReleased();
	ReleaseRope();
	StopHang();

	// mj - ladder
	GrabLadder(false);
	// mj - pulley
	//GrabPulley(false);
	// mj - numberlock
	ClickLockWheel(false);
}

void AMainCharacter::Die()
{
	if (bIsWithDoctorWhenDie && !bIsDead)
	{
		DefaultWidget->PlayAnimation(DefaultWidget->FadeOut);
		bIsDead = true;
	}
	else if(!bIsWithDoctorWhenDie && !bIsDead)
	{
		DefaultWidget->PlayAnimation(DefaultWidget->FadeOut_Doctor);
		bIsDead = true;
	}

	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SkeletalMesh->SetAllBodiesSimulatePhysics(true);
	if (bIsFP)
		SwitchCamera();
	if (OnDie.IsBound() == true)
	{
		OnDie.Broadcast();
	}
}

void AMainCharacter::ESCMenu()
{
	bool bIsGamePaused = UGameplayStatics::IsGamePaused(GetWorld());
	if (bIsGamePaused)
	{
		if (ESCMenuWidget)
		{
			UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());
			ESCMenuWidget->ResumeGame();
		}
	}
	else
	{
		if (ESCMenuWidgetClass)
		{
			ESCMenuWidget = Cast<UESCMenuWidget>(CreateWidget(GetWorld(), ESCMenuWidgetClass));
			if (ESCMenuWidget)
			{
				ESCMenuWidget->AddToViewport();
				UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);
				UGameplayStatics::SetGamePaused(GetWorld(), true);
				UWidgetBlueprintLibrary::SetInputMode_GameAndUI(UGameplayStatics::GetPlayerController(GetWorld(), 0), ESCMenuWidget);
			}
		}
	}
}

void AMainCharacter::FlashLight()
{
	if (bIsDefaultStateNotIncludingCrouching && bHaveFlashLight)
	{
		if (SoundManager)
		{
			SoundManager->MakeSoundTurnOnFlashLight(SkeletalMesh->GetSocketLocation(FName("FlashLightSocket")));
		}

		if (bIsFP && bIsFlashMode)
		{
			FlashLight_FP->SetVisibility(false);
			SpotLight_FP->SetVisibility(false);
			FlashLight_SV->SetVisibility(false);
			SpotLight_SV->SetVisibility(false);
		}
		else if (!bIsFP && bIsFlashMode)
		{
			FlashLight_SV->SetVisibility(false);
			SpotLight_SV->SetVisibility(false);
			FlashLight_FP->SetVisibility(false);
			SpotLight_FP->SetVisibility(false);
		}
		else if (bIsFP && !bIsFlashMode)
		{
			FlashLight_SV->SetVisibility(false);
			SpotLight_SV->SetVisibility(false);
			FlashLight_FP->SetVisibility(false);
			SpotLight_FP->SetVisibility(true);
		}
		else if (!bIsFP && !bIsFlashMode)
		{
			FlashLight_SV->SetVisibility(true);
			SpotLight_SV->SetVisibility(true);
			FlashLight_FP->SetVisibility(false);
			SpotLight_FP->SetVisibility(false);
		}
		bIsFlashMode = !bIsFlashMode;
	}
}

void AMainCharacter::OpenKeyExplanationWidget()
{
	bool bIsGamePaused = UGameplayStatics::IsGamePaused(GetWorld());
	if (bIsGamePaused)
	{
		if (KeyExplanationWidget)
		{
			UGameplayStatics::SetGamePaused(GetWorld(), false);
			KeyExplanationWidget->RemoveFromParent();
		}
	}
	else
	{
		if (_KeyExplanationWidget)
		{
			KeyExplanationWidget = Cast<UKeyExplanationWidget>(CreateWidget(GetWorld(), _KeyExplanationWidget));
			if (KeyExplanationWidget)
			{
				KeyExplanationWidget->AddToViewport();
				UGameplayStatics::SetGamePaused(GetWorld(), true);
			}
		}
	}
}

void AMainCharacter::DOF()
{
	if (bIsFP)
	{
		FVector StartLoc = FirstPersonCameraComponent->GetComponentLocation();
		FVector EndLoc = FirstPersonCameraComponent->GetComponentLocation() + FirstPersonCameraComponent->GetForwardVector() * 1000.0f;
		TArray<AActor*> ToIgnore;
		FHitResult OutHit;

		bool bIsOutHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartLoc, EndLoc, ETraceTypeQuery::TraceTypeQuery1, false,
			ToIgnore, EDrawDebugTrace::None, OutHit, true);
		if (bIsOutHit)
		{
			FVector Dist = OutHit.Location - StartLoc;
			FocusDistance = Dist.Size();
		}
		else
		{
			FocusDistance = 1000.0f;
		}
		FocusDistance = UKismetMathLibrary::FInterpTo(FocusDistance, TargetFocusDistance, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 60.0f);
		TargetFocusDistance = FocusDistance;
	}
}

void AMainCharacter::DefaultWidgetManager()
{
	if (bIsFP && DefaultWidget && DefaultWidget->FP_Point->GetVisibility() == ESlateVisibility::Hidden)
	{
		DefaultWidget->FP_Point->SetVisibility(ESlateVisibility::Visible);
	}
	else if (!bIsFP && DefaultWidget && DefaultWidget->FP_Point->GetVisibility() == ESlateVisibility::Visible)
	{
		DefaultWidget->FP_Point->SetVisibility(ESlateVisibility::Hidden);
	}
	bCantSwitchView = (!bIsControllable || bIsHangingRope || bIsSliding || bIsOnPole || bIsPickingUp || bIsGrabbingThrowable || bIsAttachedAtMovable || bIsOnLedge || bIsOnLadder || bIsGrabbingPulley || bIsAttachedAtPullBoard || bIsActivatingValve);
	if (bCantSwitchView)
	{
		DefaultWidget->Image_CanSwitchView->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		DefaultWidget->Image_CanSwitchView->SetVisibility(ESlateVisibility::Hidden);
	}
}


void AMainCharacter::CrouchManager()
{
	FVector StartLoc = TriggerCapsuleComponent->GetRelativeLocation();
	FVector EndLoc = TriggerCapsuleComponent->GetRelativeLocation() + FVector(0.0f, 0.0f, CapsuleDefaultHeight - 20.0f);
	TArray<AActor*> ToIgnore;
	FHitResult OutHit;
	bCanStand = !UKismetSystemLibrary::SphereTraceSingle(GetWorld(), StartLoc, EndLoc, 25.0f, ETraceTypeQuery::TraceTypeQuery1, false,
		ToIgnore, EDrawDebugTrace::None, OutHit, true);
	if (!bIsPressingCrouchingKey && !bIsGrabbingThrowable)
	{
		if (bCanStand)
		{
			if (bIsFP && UKismetMathLibrary::NearlyEqual_FloatFloat(TriggerCapsuleComponent->GetScaledCapsuleHalfHeight(), CrouchHeight, 0.01f))
			{
				SmoothCrouchCurveTimeline->Reverse();
				UnCrouch();
			}
			else if (!bIsFP && UKismetMathLibrary::NearlyEqual_FloatFloat(TriggerCapsuleComponent->GetScaledCapsuleHalfHeight(), CrouchHeight, 0.01f))
			{
				UnCrouch();
			}
			bIsCrouching = false;
			bIsHiding = false;
		}
		else
		{
			if (!bIsFP)
			{
				bIsCrouching = true;
				Crouch();
			}
			else
			{
				bIsCrouching = true;
			}
		}
	}
}
bool AMainCharacter::CanStand()
{
	return bCanStand;
}


void AMainCharacter::FallingManager()
{
	CharacterMovementComponent->IsFalling() ? bIsFalling = true : bIsFalling = false;
	CharacterMovementComponent->IsFalling() ? CharacterMovementComponent->bOrientRotationToMovement = false : CharacterMovementComponent->bOrientRotationToMovement = true;
	if (bIsFalling)
	{
		bIsCrouching = false;
		UnCrouch();

		FHitResult OutHit;
		TArray<AActor*> Arr;
		bool bCanLand = UKismetSystemLibrary::LineTraceSingle(GetWorld(), TriggerCapsuleComponent->GetRelativeLocation(), TriggerCapsuleComponent->GetRelativeLocation() + FVector(0.0f, 0.0f, -120.0f),
			ETraceTypeQuery::TraceTypeQuery1, false, Arr, EDrawDebugTrace::None, OutHit, true, FLinearColor::Red);
		if (!bCanLand)
		{
			return;
		}

		if ((TriggerCapsuleComponent->GetComponentVelocity().Z <= -900.0f && TriggerCapsuleComponent->GetComponentVelocity().Z > -1050.0f && M_HardLanding && !bIsFP))
		{
			PlayAnimMontage(M_HardLanding, 1.0f);
			bIsControllable = false;

			FTimerHandle WaitHandle;
			GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
				{
					bIsControllable = true;
				}), 2.0f, false);
			
		}
		//	Falling Die
		else if (TriggerCapsuleComponent->GetComponentVelocity().Z <= -1050.0f)
		{
			Die();
		}
		
	}
	else if (!bIsFalling && bIsPressingCrouchingKey)
	{
		if (!bIsFP)
		{
			Crouch();
		}
		bIsCrouching = true;
	}
	else if (!bIsFalling)
	{
		bPressJumpKey = false;
	}
}

void AMainCharacter::GrapRope(AMaster_Rope* Rope, float HangingLocZ)
{
	if (bIsDefaultState)
	{
		if (bIsFP)
			SwitchCamera();
		TurnOffFlashLight();
		float HangingYaw = 0.0f;
		if (Cast<ARope_NormalRope>(Rope))
		{
			_Rope = Cast<ARope_NormalRope>(Rope);
			bIsStaticRope = false;

			HangingYaw = TriggerCapsuleComponent->GetRelativeRotation().Yaw;
		}
		else if (Cast<ARope_Handle>(Rope))
		{
			_Rope = Cast<ARope_Handle>(Rope);
			bIsStaticRope = true;

			float ForwardBackFloat;
			FVector::DotProduct(_Rope->CapsuleComponent->GetForwardVector(), TriggerCapsuleComponent->GetForwardVector()) > 0 ? ForwardBackFloat = 0.0f : ForwardBackFloat = 180.0f;
			HangingYaw = _Rope->CapsuleComponent->GetRelativeRotation().Yaw + ForwardBackFloat;
		}

		FVector CharacterVelocity = CharacterMovementComponent->Velocity;

		CharacterMovementComponent->StopMovementImmediately();
		FAttachmentTransformRules AttachRule = FAttachmentTransformRules(EAttachmentRule::KeepWorld, true);
		TriggerCapsuleComponent->AttachToComponent(_Rope->CableEnd, AttachRule);
		bIsHangingRope = true;
		FVector HangingLoc = _Rope->CapsuleComponent->GetRelativeLocation() - FVector(0.0f, 0.0f, HangingLocZ);

		FRotator HangingRot = UKismetMathLibrary::MakeRotator(_Rope->CapsuleComponent->GetRelativeRotation().Roll, _Rope->CapsuleComponent->GetRelativeRotation().Pitch, HangingYaw);

		_Rope->CableEnd->AddForce(CharacterVelocity * 7000, NAME_None, false);

		FLatentActionInfo Info;
		Info.CallbackTarget = this;
		UKismetSystemLibrary::MoveComponentTo(TriggerCapsuleComponent, HangingLoc, HangingRot, true, true, 0.2f, false, EMoveComponentAction::Type::Move, Info);
	}
}

void AMainCharacter::ReleaseRope()
{
	if (bIsHangingRope)
	{
		FDetachmentTransformRules DetachRule = FDetachmentTransformRules(EDetachmentRule::KeepWorld, true);
		TriggerCapsuleComponent->DetachFromComponent(DetachRule);

		FVector Loc = GetActorLocation();
		FRotator Rot = FRotator(0.0f, GetActorRotation().Yaw, 0.0f);
		FLatentActionInfo Info;
		Info.CallbackTarget = this;
		UKismetSystemLibrary::MoveComponentTo(TriggerCapsuleComponent, Loc, Rot, false, false, 0.0f, false, EMoveComponentAction::Type::Move, Info);

		FVector LaunchVelocity = _Rope->CableEnd->GetPhysicsLinearVelocity();
		LaunchCharacter(FVector(LaunchVelocity.X * 1.5f, LaunchVelocity.Y * 1.5f, 0.0f), false, false);

		_Rope = nullptr;
		bIsHangingRope = false;
	}
}

void AMainCharacter::CharacterMaxWalkSpeedManager()
{
	if (bIsCrouching)
	{
		CharacterMovementComponent->MaxWalkSpeed = DefaultCrouchSpeed;
	}
	else if (bIsSprinting)
	{
		CharacterMovementComponent->MaxWalkSpeed = DefaultSprintSpeed;
	}
	else
	{
		CharacterMovementComponent->MaxWalkSpeed = DefaultWalkSpeed;
	}
}

void AMainCharacter::LedgeManager()
{
	ForwardTrace(GetActorLocation());
	HeightTrace(20, &bCanLedgeRightHand);
	HeightTrace(-12, &bCanLedgeLeftHand);
	// for resist ledge
	ResetLedge();
}

void AMainCharacter::MoveLedgeManager()
{
	float MoveLedgeDistance = 60.0f;

	if (bIsLedgeMoveLeft && !bIsControllable)
	{
		GoalLocLeft = TriggerCapsuleComponent->GetRelativeLocation();
		GoalLocLeft = UKismetMathLibrary::VInterpTo(GoalLocLeft, ClimbResistLocation + GetActorRightVector() * -MoveLedgeDistance, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 0.8);
		TriggerCapsuleComponent->SetRelativeLocation(GoalLocLeft);
	}
	else if (bIsLedgeMoveRight && !bIsControllable)
	{
		GoalLocRight = TriggerCapsuleComponent->GetRelativeLocation();
		GoalLocRight = UKismetMathLibrary::VInterpTo(GoalLocRight, ClimbResistLocation + GetActorRightVector() * MoveLedgeDistance, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 0.8);
		TriggerCapsuleComponent->SetRelativeLocation(GoalLocRight);
	}
}

void AMainCharacter::LedgeMove()
{
	if (bIsOnLedge && bIsControllable)
	{
		if (PlayerController->IsInputKeyDown("A"))
		{
			CheckMoveLedge("A");
		}
		else if (PlayerController->IsInputKeyDown("D"))
		{
			CheckMoveLedge("D");
		}
	}
}

void AMainCharacter::CheckMoveLedge(FString Key)
{
	if (MoveLedgeHeightTrace(20, Key) && MoveLedgeHeightTrace(-12, Key) && MoveLedgeForwardTrace(GetActorLocation(), Key))
	{
		if (Key == "A" && M_LedgeMoveLeft)
		{
			ClimbResistLocation += GetActorRightVector() * -60;

			bIsResisted = true;
			bDoOnceLedge = false;
			bIsControllable = false;
			PlayAnimMontage(M_LedgeMoveLeft);
			bIsLedgeMoveLeft = true;
		}
		else if (Key == "D" && M_LedgeMoveRight)
		{
			ClimbResistLocation += GetActorRightVector() * 60;

			bIsResisted = true;
			bDoOnceLedge = false;
			bIsControllable = false;
			PlayAnimMontage(M_LedgeMoveRight);
			bIsLedgeMoveRight = true;
		}
	}
}

void AMainCharacter::ForwardTrace(FVector StartLocation)
{
	FVector StartLoc = StartLocation;
	FVector EndLoc = StartLoc + FVector(GetActorRotation().Vector().X * 150, GetActorRotation().Vector().Y * 150, GetActorRotation().Vector().Z);
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);

	FHitResult HitResult;

	bool hit = UKismetSystemLibrary::SphereTraceSingle
	(
		GetWorld(),
		StartLoc,
		EndLoc,
		10.0f,
		TraceTypeQuery3,
		false,
		IgnoreActors,
		EDrawDebugTrace::None,
		HitResult,
		true
	);
	if (hit)
	{
		WallLoc = HitResult.Location;
		WallNorm = HitResult.ImpactNormal;
	}
}

void AMainCharacter::HeightTrace(float offset, bool* bCanLedgeHand)
{
	FVector StartLoc = GetActorLocation() + GetActorRotation().Vector() * 30 + FVector(0.0f, 0.0f, 120.0f) + TriggerCapsuleComponent->GetRightVector() * offset;
	FVector EndLoc = StartLoc - FVector(0.0f, 0.0f, 120.0f);
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	FHitResult HitResult;

	bool hit = UKismetSystemLibrary::SphereTraceSingle
	(
		GetWorld(),
		StartLoc,
		EndLoc,
		10.0,
		TraceTypeQuery3,
		true,
		IgnoreActors,
		EDrawDebugTrace::None,
		HitResult,
		true
	);

	if (hit)
	{
		*bCanLedgeHand = true;
		HeightLoc = HitResult.Location;
		Range = SkeletalMesh->GetSocketLocation("PelvisSocket").Z - HeightLoc.Z;
		if (Range >= -120.0f && Range <= -110.0f)
		{
			if (!bIsOnLedge && bIsFalling)
			{
				GrabLedge();
			}
		}
	}
	else
	{
		*bCanLedgeHand = false;
	}
}

bool AMainCharacter::MoveLedgeForwardTrace(FVector StartLocation, FString Key)
{
	float gap;
	if (Key == "A")
		gap = -60;
	else if (Key == "D")
		gap = 60;
	FVector StartLoc = StartLocation + TriggerCapsuleComponent->GetRightVector() * gap;
	FVector EndLoc = StartLoc + FVector(GetActorRotation().Vector().X * 150, GetActorRotation().Vector().Y * 150, GetActorRotation().Vector().Z);
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);

	FHitResult HitResult;

	bool hit = UKismetSystemLibrary::SphereTraceSingle
	(
		GetWorld(),
		StartLoc,
		EndLoc,
		10.0f,
		TraceTypeQuery3,
		false,
		IgnoreActors,
		EDrawDebugTrace::None,
		HitResult,
		true
	);
	bool bIsFloat = UKismetMathLibrary::NearlyEqual_FloatFloat(FVector::DotProduct(WallNorm - HitResult.Normal, WallNorm), 0, 0.1);
	if (hit && bIsFloat)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool AMainCharacter::MoveLedgeHeightTrace(float offset, FString Key)
{
	float gap;
	if (Key == "A")
		gap = -60;
	else if (Key == "D")
		gap = 60;
	FVector StartLoc = GetActorLocation() + GetActorRotation().Vector() * 30 + FVector(0.0f, 0.0f, 120.0f) + TriggerCapsuleComponent->GetRightVector() * (offset + gap);
	FVector EndLoc = StartLoc - FVector(0.0f, 0.0f, 120.0f);
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	FHitResult HitResult;

	bool hit = UKismetSystemLibrary::SphereTraceSingle
	(
		GetWorld(),
		StartLoc,
		EndLoc,
		10.0,
		TraceTypeQuery3,
		true,
		IgnoreActors,
		EDrawDebugTrace::None,
		HitResult,
		true
	);
	if (hit && UKismetMathLibrary::NearlyEqual_FloatFloat(HeightLoc.Z, HitResult.Location.Z, 0.1))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void AMainCharacter::GrabLedge()
{
	if (bIsPressLMB && !bIsOnLedge && bCanLedgeLeftHand && bCanLedgeRightHand && bIsDefaultState && bIsControllable)
	{
		if (bIsFP)
			SwitchCamera();
		TurnOffFlashLight();
		FLatentActionInfo Info;

		float NormLoc_X = WallNorm.X * 22 + WallLoc.X;
		float NormLoc_Y = WallNorm.Y * 22 + WallLoc.Y;
		FVector TargetRelativeLocation = UKismetMathLibrary::MakeVector(NormLoc_X, NormLoc_Y, HeightLoc.Z - CapsuleDefaultHeight + 10.0f);
		FRotator TargetRelativeRotation = UKismetMathLibrary::MakeRotFromX(-1.0f * WallNorm);
		Info.CallbackTarget = this;

		TEnumAsByte< EMoveComponentAction::Type> MoveAction;
		UKismetSystemLibrary::MoveComponentTo
		(
			TriggerCapsuleComponent,
			TargetRelativeLocation,
			TargetRelativeRotation,
			true,
			true,
			0.2,
			false,
			MoveAction,
			Info
		);
		CharacterMovementComponent->SetMovementMode(MOVE_Flying, 0);
		bIsOnLedge = true;

		PlayAnimMontage(M_ClimbLedge, 0.0f, NAME_None);
		SkeletalMesh->GetAnimInstance()->Montage_Pause(M_ClimbLedge);
		CharacterMovementComponent->StopMovementImmediately();

		// For Climb Resist
		ClimbResistLocation = TriggerCapsuleComponent->GetComponentLocation() /*+ FVector(0.0f, 0.0f, 10.0f)*/;
		ClimbResistRotation = TriggerCapsuleComponent->GetComponentRotation();
	}
}

void AMainCharacter::StopHang()
{
	if (bIsControllable && bIsOnLedge && !bIsPressLMB)
	{
		StopAnimMontage(M_ClimbLedge);
		CharacterMovementComponent->SetMovementMode(MOVE_Falling, 0);
		bIsOnLedge = false;
		TriggerCapsuleComponent->SetCapsuleRadius(CapsuleDefaultRadius);
	}
}
void AMainCharacter::ClimbUp()
{
	if (bIsOnLedge && bIsControllable)
	{
		FVector StartLoc = GetActorLocation() + GetActorRotation().Vector() * 30 + FVector(0.0f, 0.0f, 60.0f);
		FVector EndLoc = StartLoc + GetActorRotation().Vector() * 100;
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(this);
		FHitResult OutHit;

		bool hit = UKismetSystemLibrary::SphereTraceSingle
		(
			GetWorld(),
			StartLoc,
			EndLoc,
			5.0,
			TraceTypeQuery1,
			true,
			IgnoreActors,
			EDrawDebugTrace::None,
			OutHit,
			true
		);

		if (bDoOnceLedge && bIsOnLedge && bIsPressLMB)
		{
			if (!hit || OutHit.Distance >= 60.0f)
			{
				bDoOnceLedge = false;
				PlayAnimMontage(M_ClimbLedge, 1.0f, NAME_None);
				bIsControllable = false;
			}
			else if (hit && OutHit.Distance < 60.0f)
			{
				bDoOnceLedge = false;
				PlayAnimMontage(M_ClimbResistLedge, 1.0f, NAME_None);
				bIsControllable = false;
				bIsResisted = true;
			}
		}
	}
}

void AMainCharacter::ResetLedge()
{
	if (bIsResisted)
	{
		if (!bIsControllable && bIsOnLedge && (GetCurrentMontage() != M_ClimbResistLedge && GetCurrentMontage() != M_LedgeMoveLeft && GetCurrentMontage() != M_LedgeMoveRight))
		{
			bDoOnceLedge = true;
			bIsResisted = false;

			if (!bIsPressLMB)   // During Playing Resist Climb Animation, LMB is Released 
			{
				bIsOnLedge = false;
				CharacterMovementComponent->SetMovementMode(MOVE_Walking, 0);
				TriggerCapsuleComponent->SetCapsuleRadius(CapsuleDefaultRadius);
			}
			else    // During Playing Resist Climb Animation, LMB Keep Pressed
			{
				PlayAnimMontage(M_ClimbLedge, 0.0f, NAME_None);
				SkeletalMesh->GetAnimInstance()->Montage_Pause(M_ClimbLedge);
				CharacterMovementComponent->StopMovementImmediately();
				bIsOnLedge = true;

				FLatentActionInfo Info;
				Info.CallbackTarget = this;
				UKismetSystemLibrary::MoveComponentTo
				(
					TriggerCapsuleComponent,
					ClimbResistLocation,
					ClimbResistRotation,
					true,
					true,
					0.2,
					false,
					EMoveComponentAction::Move,
					Info
				);
			}
		}
	}
}

void AMainCharacter::MakeNoise(USoundCue* FootStep, FVector Location)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), FootStep, Location);
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), Location, 1.0f, this, 0.0f, tags::NoiseTag);
}

void AMainCharacter::PressLMBForMovable()
{
	if (bIsDefaultState && !bIsFalling)
	{
		FVector StartLoc1 = TriggerCapsuleComponent->GetRelativeLocation() + TriggerCapsuleComponent->GetRightVector() * 20.0f + TriggerCapsuleComponent->GetUpVector() * 30.0f;
		FVector EndLoc1 = StartLoc1 + TriggerCapsuleComponent->GetForwardVector() * 40.0f;
		TArray<AActor*> ToIgnore1;
		FHitResult OutHit1;
		bool bIsOutHit1 = UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartLoc1, EndLoc1, ETraceTypeQuery::TraceTypeQuery1, false,
			ToIgnore1, EDrawDebugTrace::None, OutHit1, true);

		FVector StartLoc2 = TriggerCapsuleComponent->GetRelativeLocation() + TriggerCapsuleComponent->GetRightVector() * -20.0f + TriggerCapsuleComponent->GetUpVector() * 30.0f;
		FVector EndLoc2 = StartLoc2 + TriggerCapsuleComponent->GetForwardVector() * 40.0f;
		TArray<AActor*> ToIgnore2;
		FHitResult OutHit2;
		bool bIsOutHit2 = UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartLoc2, EndLoc2, ETraceTypeQuery::TraceTypeQuery1, false,
			ToIgnore2, EDrawDebugTrace::None, OutHit2, true);

		FVector StartLoc3 = TriggerCapsuleComponent->GetRelativeLocation();
		FVector EndLoc3 = StartLoc3 + TriggerCapsuleComponent->GetUpVector() * -100.0f;
		TArray<AActor*> ToIgnore3;
		FHitResult OutHit3;
		bool bIsOutHit3 = UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartLoc3, EndLoc3, ETraceTypeQuery::TraceTypeQuery1, false,
			ToIgnore3, EDrawDebugTrace::None, OutHit3, true);

		if (bIsOutHit1 && bIsOutHit2)
		{
			if (bIsOutHit3)
			{
				if (OutHit3.GetActor()->GetClass()->GetFName() == "BP_Drawer_C")
				{
					return;
				}
			}
			MovableObj = Cast<AMaster_Movable>(OutHit1.Actor);
			if (MovableObj)
			{
				if (!bDoOncePushPull)
				{
					if (bIsFP)
					{
						SwitchCamera();
					}
					TurnOffFlashLight();
					CharacterMovementComponent->bOrientRotationToMovement = false;
					bIsAttachedAtMovable = true;
					MovableObj->StaticMesh->SetSimulatePhysics(true);

					FVector BoxLoc = TriggerCapsuleComponent->GetComponentLocation();
					MovableObj->Box->SetWorldLocation(BoxLoc + OutHit1.ImpactNormal * 30);
					MovableObj->Box->SetCollisionProfileName("Custom...");
					MovableObj->Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
					MovableObj->Box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		
					FLatentActionInfo Info;
					Info.CallbackTarget = this;
					Info.ExecutionFunction = "MovableMoveComponentToFunc";
					Info.Linkage = 0;
		
					UKismetSystemLibrary::MoveComponentTo
					(
						TriggerCapsuleComponent,
						TriggerCapsuleComponent->GetRelativeLocation() + OutHit1.ImpactNormal * 30,
						UKismetMathLibrary::MakeRotFromX(OutHit1.Normal * -1),
						true,
						true,
						0.2,
						false,
						EMoveComponentAction::Move,
						Info
					);
					
					bDoOncePushPull = true;
				}
			}
		}
	}
}
void AMainCharacter::MovableMoveComponentToFunc()
{
	if (bIsAttachedAtMovable)
	{
		TriggerCapsuleComponent->AttachToComponent(MovableObj->StaticMesh, FAttachmentTransformRules::KeepWorldTransform);
	}
}

void AMainCharacter::ReleaseLMBForMovable()
{

	if (!bIsFP && bIsAttachedAtMovable)
	{
		MovableObj->Box->SetRelativeLocation(MovableObj->DefaultBoxLocation);
		MovableObj->Box->SetCollisionProfileName("NoCollision");
		MovableObj->Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		TriggerCapsuleComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		CharacterMovementComponent->bOrientRotationToMovement = true;
		bIsAttachedAtMovable = false;
		MovableObj->StaticMesh->SetSimulatePhysics(false);
		MovableObj = nullptr;
		bIsLeftHandHitWall = false;
		bIsRightHandHitWall = false;
		bDoOncePushPull = false;

		FLatentActionInfo Info;
		Info.CallbackTarget = this;
	}

}

void AMainCharacter::PushPullManager()
{
	if (bIsAttachedAtMovable)
	{
		MovableObjPushPull();

		ForwardHandIK();
	}
}

void AMainCharacter::ForwardHandIK()
{
	TTuple<FVector, bool> arm_lInfo = PushPullHandIK(FName("upperarm_l"), 100.0f);
	TTuple<FVector, bool> arm_rInfo = PushPullHandIK(FName("upperarm_r"), 100.0f);
	IKLeftHandLocation = arm_lInfo.Get<0>();
	IKRightHandLocation = arm_rInfo.Get<0>();
	bIsLeftHandHitWall = arm_lInfo.Get<1>();
	bIsRightHandHitWall = arm_rInfo.Get<1>();
}

void AMainCharacter::MovableObjPushPull()
{
	if (!(XAxisValue == 0.0f && YAxisValue == 0.0f) && CanPushOrPullDegree())
	{
		const int Power = 100;
		FVector StartLoc = TriggerCapsuleComponent->GetComponentLocation();
		FVector EndLoc = StartLoc + TriggerCapsuleComponent->GetForwardVector() * Power;
		TArray<AActor*> ToIgnore;
		FHitResult OutHit;

		bool bIsOutHit = UKismetSystemLibrary::LineTraceSingle(
			GetWorld(), 
			StartLoc,
			EndLoc,
			ETraceTypeQuery::TraceTypeQuery1,
			false,
			ToIgnore,
			EDrawDebugTrace::None,
			OutHit, 
			true
		);

		if (bIsOutHit)
		{
			MovableObjNorm = OutHit.Normal;
			if (GetIsPushOrPull())   // Push
			{
				MovableObj->StaticMesh->AddWorldOffset(FVector(XAxisValue, YAxisValue, 0.0f));
				MovableObj->StaticMesh->AddWorldRotation(FRotator(0.0f, PushOrPullRotate(true), 0.0f));

			}
			else // Pull
			{
				MovableObj->StaticMesh->AddWorldOffset(FVector(XAxisValue, YAxisValue, 0.0f));
				MovableObj->StaticMesh->AddWorldRotation(FRotator(0.0f, PushOrPullRotate(false), 0.0f));
			}
		}
	}
}
// can push?
bool AMainCharacter::CanPushOrPullDegree()
{
	FVector ForwardVec = TriggerCapsuleComponent->GetForwardVector();
	float cos = FMath::Cos(FVector2D::DotProduct(FVector2D(XAxisValue, YAxisValue), FVector2D(ForwardVec.X, ForwardVec.Y)));
	return !UKismetMathLibrary::NearlyEqual_FloatFloat(cos, 1.0f, 0.1f);
}
// push or pull
bool AMainCharacter::GetIsPushOrPull()
{
	return FVector2D::DotProduct(FVector2D(XAxisValue, YAxisValue), FVector2D(MovableObjNorm.X, MovableObjNorm.Y)) < 0.0f;
}
// calculate rotation value
float  AMainCharacter::PushOrPullRotate(bool bIsPush)
{
	FVector Vec = UKismetMathLibrary::SelectFloat(-1.0f, 1.0f, bIsPush) * MovableObjNorm;
	float DegreeGap = UKismetMathLibrary::DegAtan2(XAxisValue, YAxisValue) - UKismetMathLibrary::DegAtan2(Vec.X, Vec.Y);
	DegreeGap = UKismetMathLibrary::SelectFloat(DegreeGap + 360.0f, DegreeGap, DegreeGap < -180.0f);
	DegreeGap = UKismetMathLibrary::SelectFloat(DegreeGap - 360.0f, DegreeGap, DegreeGap > 180.0f);
	float RotateValue = DegreeGap / ((FMath::Abs(DegreeGap) / 90.0f) * -500.0f);
	return RotateValue;
}
// push or pull
float AMainCharacter::GetPushPullDir()
{
	return FVector2D::DotProduct(FVector2D(XAxisValue, YAxisValue), FVector2D(MovableObjNorm.X, MovableObjNorm.Y));
}

TTuple<FVector, bool> AMainCharacter::PushPullHandIK(FName SocketName, float Distance)
{
	FVector StartLoc = SkeletalMesh->GetSocketLocation(FName(SocketName));
	FVector EndLoc = (StartLoc + TriggerCapsuleComponent->GetForwardVector() * Distance);
	TArray<AActor*> ToIgnore;
	FHitResult OutHit;

	bool bIsOutHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartLoc, EndLoc, ETraceTypeQuery::TraceTypeQuery1, false,
		ToIgnore, EDrawDebugTrace::None, OutHit, true);
	if (bIsOutHit)
	{
		if (bIsAttachedAtPullBoard && SlicableMesh)
			return MakeTuple(FVector(OutHit.Location.X, OutHit.Location.Y, SlicableMesh->HandIKLoc->GetComponentLocation().Z), true);
		else
			return MakeTuple(OutHit.Location, true);
	}
	else
	{
		return MakeTuple(FVector(0.0f), false);
	}
}

void AMainCharacter::ThrowManager()
{
	if (!bIsFalling && !bIsFP && !bIsCrouching && bIsThrowKeyDown)
	{
		ThrowGauge = UKismetMathLibrary::FClamp(ThrowGauge + 0.005, 0.5f, 1.0f);
	}
}

void AMainCharacter::PressLMBForThrow()
{
	FVector StartLoc = TriggerCapsuleComponent->GetRelativeLocation() + TriggerCapsuleComponent->GetForwardVector() * 30.0f + FVector(0.0f, 0.0f, -50.0f);
	FVector EndLoc = StartLoc;
	TArray<AActor*> ToIgnore;
	FHitResult OutHit;
	bool bIsThrowable = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		StartLoc,
		EndLoc,
		10.0f,
		ETraceTypeQuery::TraceTypeQuery5,
		false,
		ToIgnore,
		EDrawDebugTrace::None,
		OutHit,
		true);
	if (bIsThrowable)
	{
		if (Cast<AMaster_Throwable>(OutHit.Actor))
		{
			if (bIsFP)
				SwitchCamera();
			TurnOffFlashLight();
			ThrowableObject = Cast<AMaster_Throwable>(OutHit.Actor);
			bCanThrow = true;
		}
		else
		{
			bCanThrow = false;
		}
	}
	else
	{
		bCanThrow = false;
	}

	if (bIsControllable && bCanThrow)
	{
		bReleasedLMB = false;
		bIsGrabbingThrowable = true;
		bIsControllable = false;
		if (M_PickUp)
		{
			PlayAnimMontage(M_PickUp);
			bIsPickingUp = true;
			bIsHoldingThrowable = true;
		}
	}
}

void AMainCharacter::ReleaseLMBForThrow()
{
	bReleasedLMB = true;
	DropDown();
}

void AMainCharacter::PressLMBForClimbingPole()
{
	FVector StartLoc = TriggerCapsuleComponent->GetRelativeLocation() + TriggerCapsuleComponent->GetForwardVector() * 40.0f + FVector(0.0f, 0.0f, -30.0f);
	FVector EndLoc = StartLoc;
	TArray<AActor*> ToIgnore;
	FHitResult OutHit;
	bool bIsPole = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		StartLoc,
		EndLoc,
		40.0f,
		ETraceTypeQuery::TraceTypeQuery7,
		false,
		ToIgnore,
		EDrawDebugTrace::None,
		OutHit,
		true);
	if (bIsPole)
	{
		if (bIsFP)
			SwitchCamera();
		TurnOffFlashLight();
		bIsOnPole = true;
		CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Flying);
		_Pole = Cast<AMaster_Pole>(OutHit.Actor);
		_Pole->StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CharacterMovementComponent->StopMovementImmediately();

		FVector HangingLoc = FVector(OutHit.Actor->GetActorLocation().X, OutHit.Actor->GetActorLocation().Y, OutHit.Location.Z) + GetActorForwardVector() * -15 + FVector(0.0f, 0.0f, 60.0f);
		FRotator HangingRot = GetActorRotation();
		FLatentActionInfo Info;
		Info.CallbackTarget = this;
		UKismetSystemLibrary::MoveComponentTo(
			TriggerCapsuleComponent,
			HangingLoc,
			HangingRot,
			true,
			true,
			0.2f,
			false,
			EMoveComponentAction::Type::Move,
			Info);
	}
}

void AMainCharacter::ReleaseLMBClimbingPole()
{
	if (bIsOnPole)
	{
		if (bIsFalling)
		{
			CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Falling);
		}
		else
		{
			CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);
		}
		bIsOnPole = false;
		_Pole->StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		_Pole = nullptr;

		FVector HangingLoc = GetActorLocation();
		FRotator HangingRot = FRotator(GetActorRotation().Roll, GetActorRotation().Yaw, GetActorRotation().Pitch);
		FLatentActionInfo Info;
		Info.CallbackTarget = this;
		UKismetSystemLibrary::MoveComponentTo(
			TriggerCapsuleComponent,
			HangingLoc,
			HangingRot,
			true,
			true,
			0.2f,
			false,
			EMoveComponentAction::Type::Move,
			Info);
	}
}

void AMainCharacter::DropDown()
{
	if (bIsGrabbingThrowable && bIsControllable)
	{
		bIsGrabbingThrowable = false;
		bIsControllable = true;
		if (ThrowableObject)
		{
			ThrowableObject->DropDown();
		}
	}
}

void AMainCharacter::SwimmingManager()
{
	if (CharacterMovementComponent->MovementMode == EMovementMode::MOVE_Swimming)
	{
		if (!bIsFP)
		{
			CharacterMovementComponent->Buoyancy = 1.3f;

			FVector StartLoc = TriggerCapsuleComponent->GetComponentLocation();
			FVector EndLoc = StartLoc - TriggerCapsuleComponent->GetUpVector() * 100.0f;
			TArray<AActor*> ToIgnore;
			FHitResult OutHit;

			bool bIsOutHit = UKismetSystemLibrary::LineTraceSingle(
				GetWorld(),
				StartLoc,
				EndLoc,
				ETraceTypeQuery::TraceTypeQuery1,
				false,
				ToIgnore,
				EDrawDebugTrace::None,
				OutHit,
				true
			);

			if (bIsOutHit)
			{
				bIsWalkingOnSwimming = true;
			}
			else
			{
				bIsWalkingOnSwimming = false;
			}
		}
		else
		{
			CharacterMovementComponent->Buoyancy = 1.0f;
		}
	}
}

void AMainCharacter::OnMontageNotifyBegin()
{
	if (GetCurrentMontage() == M_PickUp)
	{
		ThrowableObject->PickUp((SkeletalMesh->GetSocketLocation(FName("hand_r")) + SkeletalMesh->GetSocketLocation(FName("hand_l"))) / 2, this);
	}
	else if (GetCurrentMontage() == M_ReadyToThrow)
	{
		SkeletalMesh->GetAnimInstance()->Montage_Pause();
	}
	else if (GetCurrentMontage() == M_ClimbLedge)
	{
		bDoOnceLedge = true;
		bIsOnLedge = false;
		bIsControllable = true;
		CharacterMovementComponent->SetMovementMode(MOVE_Walking, 0);
		bIsResisted = false;
		TriggerCapsuleComponent->SetCapsuleRadius(CapsuleDefaultRadius);
	}
}

void AMainCharacter::OnMontageBlendOut(UAnimMontage* AnimMontage, bool bInterrupted)
{
	if (AnimMontage == M_CrawlingCorpse)
	{
		PlayAnimMontage(M_CrawlToStand);
	}
	else if (AnimMontage == M_PullBoardFallDown)
	{
		PlayAnimMontage(M_FallingDownToStand);
	}
	else if (AnimMontage ==  M_FallingDownToStand)
	{
		bIsAttachedAtPullBoard = false;
		bIsControllable = true;
	}
	else if (AnimMontage == M_JumpOnWall)
	{
		TriggerCapsuleComponent->SetRelativeRotation(FRotator(0, -TriggerCapsuleComponent->GetRelativeRotation().Yaw, 0));
	}
}

void AMainCharacter::OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
	if (AnimMontage == M_PickUp)
	{
		bIsPickingUp = false;
		bIsControllable = true;
		if (bReleasedLMB)
		{
			DropDown();
		}
	}
	else if (AnimMontage == M_Throw)
	{
		bIsControllable = true;
	}
	else if (AnimMontage == M_Slide)
	{
		bIsControllable = true;
		bIsPressingCrouchingKey = false;
		bIsSliding = false;
		if (bIsPressingSprintKey)
			bIsSprinting = true;
		else
			bIsSprinting = false;
		CharacterMovementComponent->BrakingDecelerationWalking = 2048.0f;
	}
	else if (AnimMontage == M_PullBoard)
	{
		bIsPlayedPullBoardAnim = false;
	}
	else if (AnimMontage ==  M_FallingDownToStand)
	{
		EndPullBoard();
	}
	else if (AnimMontage == M_CrawlToStand)
	{
		bIsControllable = true;
		DefaultWidget->PlayAnimation(DefaultWidget->Controllable);
	}
	else if (AnimMontage == M_RespawnStand)
	{
		bIsControllable = true;
		DefaultWidget->PlayAnimation(DefaultWidget->Controllable);
	}
	else if (AnimMontage == M_ClimbResistLedge)
	{
		bIsControllable = true;
		if (bReleasedHangingLedge)
		{
			StopHang();
		}
	}
	else if (AnimMontage == M_LedgeMoveLeft || AnimMontage == M_LedgeMoveRight)
	{
		bIsLedgeMoveRight = false;
		bIsLedgeMoveLeft = false;
		bIsControllable = true;
		StopHang();
	}
	else if (AnimMontage == M_ClimbLedge)
	{
		if (bIsPressingSprintKey)
		{
			bIsSprinting = true;
		}
	}
	else if (AnimMontage == M_EdgeSlip)
	{
		bIsControllable = true;
	}
}

void AMainCharacter::SetupStimulus()
{
	stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("stimulus"));
	stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	stimulus->RegisterWithPerceptionSystem();
}

void AMainCharacter::TurnOffFlashLight()
{
	FlashLight_SV->SetVisibility(false);
	SpotLight_SV->SetVisibility(false);
	bIsFlashMode = false;
}

void AMainCharacter::SetVisibilityHidedActors(bool b, TArray<AActor*> Actors)
{
	for (AActor* i : Actors)
	{
		i->SetActorHiddenInGame(!b);
	}
}

void AMainCharacter::Save()
{
	GameInstance->SaveGameData->MainCharacterStruct.bIsStartingPoint = false;
	GameInstance->SaveGameData->bHaveFlashLight = bHaveFlashLight;
	DefaultWidget->PlayAnimation(DefaultWidget->Save);
}

void AMainCharacter::Reset()
{
	if (bIsTestingMode)
		bHaveFlashLight = true;
	else
		bHaveFlashLight = GameInstance->SaveGameData->bHaveFlashLight;


	switch (GameInstance->SaveGameData->SavePointType)
	{
	case ECheckPointRespawnType::None:
		GetUpWhenRespawn();
		break;
	case ECheckPointRespawnType::Crawling:
		if (M_CrawlingCorpse)
		{
			TriggerCapsuleComponent->SetRelativeRotation(FRotator(0, 90, 0));
			if (!bIsTestingMode)
			{
				PlayAnimMontage(M_CrawlingCorpse);
				bIsControllable = false;
			}
		}
		break;
	case ECheckPointRespawnType::Getup:
		GetUpWhenRespawn();
		break;
	case ECheckPointRespawnType::WalkIn:
		break;
	default:
		break;
	}

	if (bIsWithDoctorWhenDie)
	{
		DefaultWidget->PlayAnimation(DefaultWidget->FadeIn);
	}
	else
	{
		DefaultWidget->PlayAnimation(DefaultWidget->FadeIn_Doctor);
	}
}

void AMainCharacter::GetUpWhenRespawn()
{
	SkeletalMesh->SetCollisionProfileName("CharacterMesh");
	SkeletalMesh->SetSimulatePhysics(false);
	TriggerCapsuleComponent->SetWorldLocation(SkeletalMesh->GetComponentLocation());
	SkeletalMesh->AttachToComponent(TriggerCapsuleComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	SkeletalMesh->SetRelativeLocation(FVector(0, 0, -65));
	SkeletalMesh->SetRelativeRotation(FRotator(0, -90, 0));
	SetActorRotation(FRotator(0, 180, 0));
	PlayerController->SetControlRotation(FRotator(0, 0, 180.0f));
	SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
	PlayAnimMontage(M_RespawnStand);
	bIsControllable = false;
}

void AMainCharacter::ResetHidedActors()
{
	HidedActors.Empty();
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("hide"), HidedActors);
	if (bIsFP)
	{
		SetVisibilityHidedActors(true, HidedActors);
	}
	else
	{
		SetVisibilityHidedActors(false, HidedActors);
	}

	//mj - fast focus
	FastFocusHidedActors.Empty();
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("fastfocushide"), FastFocusHidedActors);
}

// MJ

// Camera Switching
void AMainCharacter::SetCameraToThirdPersonMode()
{
	bIsFP = false;
	CharacterMovementComponent->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = false;
}

void AMainCharacter::SetCameraUtility(AActor* CU)
{
	if (CU != nullptr)
	{
		CameraUtility = Cast<ACameraUtility>(CU);
	}
}

//mj modified
void AMainCharacter::SwitchCamera()
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 6, FColor::Cyan, FString::Printf(TEXT("sc : switch cam to tp:%d"), bCantSwitchView ? 0 : 1));

	if(!bCantSwitchView)
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 6, FColor::Cyan, FString::Printf(TEXT("sc : switch cam to tp:%d"), bCantSwitchView ? 0 : 1));

	if (!bCantSwitchView)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 6, FColor::Cyan, FString::Printf(TEXT("bisDSIC>> :%d"), bIsDefaultStateNotIncludingCrouching ? 0 : 1));

		if (CameraUtility && bIsDefaultStateNotIncludingCrouching)
		{
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Blue, TEXT("-----CameraUtility is On"));
			// to FP
			if (!bIsFP)
			{
				bIsFP = true;

				CharacterMovementComponent->bOrientRotationToMovement = true;

				// rotation
				FRotator RRot = this->GetActorRotation();
				bUseControllerRotationYaw = true;
				PlayerController->SetControlRotation(RRot);


				Cast<ACameraUtility>(CameraUtility)->ToggleFPCamera(true);

				SkeletalMesh->SetHiddenInGame(true);

				if (bIsFlashMode)
				{
					FlashLight_FP->SetVisibility(false);
					SpotLight_FP->SetVisibility(true);

				}
				else
				{
					FlashLight_FP->SetVisibility(false);
					SpotLight_FP->SetVisibility(false);
				}
				FlashLight_SV->SetVisibility(false);
				SpotLight_SV->SetVisibility(false);

				SetVisibilityHidedActors(true, HidedActors);
				if(bIsFastFocusActorHidden)
					SetVisibilityHidedActors(true, FastFocusHidedActors);
			}
			// to TP
			else if (bIsFP)
			{
				bIsFP = false;

				CharacterMovementComponent->bOrientRotationToMovement = false;



				// rotation
				FRotator RRot = this->GetActorRotation();
				bUseControllerRotationYaw = false;
				// controller rotation actor rotation
				PlayerController->SetControlRotation(FRotator(0, 0, 0));
				this->SetActorRotation(RRot);


				Cast<ACameraUtility>(CameraUtility)->ToggleFPCamera(false);

				SkeletalMesh->SetHiddenInGame(false);

				if (bIsFlashMode)
				{
					FlashLight_SV->SetVisibility(true);
					SpotLight_SV->SetVisibility(true);

				}
				else
				{
					FlashLight_SV->SetVisibility(false);
					SpotLight_SV->SetVisibility(false);
				}
				FlashLight_FP->SetVisibility(false);
				SpotLight_FP->SetVisibility(false);

				SetVisibilityHidedActors(false, HidedActors);
				if (bIsFastFocusActorHidden)
					SetVisibilityHidedActors(false, FastFocusHidedActors);
			}
		}
	}
}

// MJ
// ladder

void AMainCharacter::SetLadder(AActor* Ladder)
{
	OwnLadder = Ladder;

	if (OwnLadder)
	{
		bIsLadderReady = true;
	}
	else
	{
		bIsLadderReady = false;
	}
}

void AMainCharacter::GrabLadder(bool b)
{
	if (bIsControllable) {
		if (b && OwnLadder && TimeToEnableGrabLadder == 0.0f)
		{


			float Pi = acos(-1);

			FVector fvL = OwnLadder->GetActorForwardVector();
			FVector fvC = this->GetActorForwardVector();

			float dot = FVector::DotProduct(fvL, fvC);
			float deg = FMath::Acos(dot) / Pi * 180.f;
			if (deg < 45.0f)
			{
				FVector SLoc;
				if (Cast<ALadder>(OwnLadder)->GetIsEnableSideMove())
				{
					FVector LLoc = OwnLadder->GetActorLocation();
					FVector LRVec = OwnLadder->GetActorRightVector();
					FVector CLoc = this->GetActorLocation();

					FVector Dif = FVector(CLoc.X - LLoc.X, CLoc.Y - LLoc.Y, 0);

					float ProjLength = FVector::DotProduct(LRVec, Dif);// /sqrt(Dif.X * Dif.X + Dif.Y * Dif.Y);

					float HorizontalLimit = OwnLadder->GetActorScale().Y * 0.8f /* hitbox size */ * 50 - 20;

					if (abs(ProjLength) < HorizontalLimit)
					{
						SLoc = LLoc + LRVec * ProjLength;
					}
					else
						return;
				}




				bIsOnLadder = true;

				if (bIsFP)
					SwitchCamera();

				this->GetCapsuleComponent()->SetCapsuleRadius(30.0f);	// hard coding warning! radius for ladder


				CharacterMovementComponent->SetMovementMode(MOVE_Flying);
				CharacterMovementComponent->StopMovementImmediately();

				if (OwnLadder) {

					AxisClimbingDir = 0;

					FVector LLoc = OwnLadder->GetActorLocation();
					FVector LFwVec = OwnLadder->GetActorForwardVector();
					float Offset = Cast<ALadder>(OwnLadder)->GetGrabHeightOffset();
					float Gap = Cast<ALadder>(OwnLadder)->GetBarGap();

					// calc grap height

					float hDiff = this->GetActorLocation().Z - LLoc.Z - HOffset; //64.15
					if (hDiff < 0)
						hDiff = 0;

					int hGapCount = (int)(hDiff / Gap) + 1;

					float hGap = Gap * hGapCount;

					FLatentActionInfo Info;
					Info.CallbackTarget = this;
					if (Cast<ALadder>(OwnLadder)->GetIsEnableSideMove())
					{
						UKismetSystemLibrary::MoveComponentTo
						(
							TriggerCapsuleComponent,
							SLoc - LFwVec * 15.f + FVector(0, 0, Cast<ALadder>(OwnLadder)->GetGrabHeightOffset() + Offset + Gap * 0.5f + hGap),
							OwnLadder->GetActorRotation(),
							false,
							false,
							0.2,
							false,
							EMoveComponentAction::Move,
							Info
						);
					}
					else
					{
						UKismetSystemLibrary::MoveComponentTo
						(
							TriggerCapsuleComponent,
							LLoc - LFwVec * 15.f + FVector(0, 0, Cast<ALadder>(OwnLadder)->GetGrabHeightOffset() + Offset + Gap * 0.5f + hGap),
							OwnLadder->GetActorRotation(),
							false,
							false,
							0.2,
							false,
							EMoveComponentAction::Move,
							Info
						);
					}
				}
			}
		}
		else if (!b && bIsOnLadder)
		{
			// Set CoolDown time
			TimeToEnableGrabLadder = 0.5f;

			bIsOnLadder = false;

			this->GetCapsuleComponent()->SetCapsuleRadius(30.0f);	// hard coding warning! restoration from radius for ladder

			CharacterMovementComponent->SetMovementMode(MOVE_Walking);
		}
	}
}

bool AMainCharacter::GetIsOnLadder()
{
	return bIsOnLadder;
}

bool AMainCharacter::GetIsClimbing()
{
	return bIsLadderClimbing;
}

bool AMainCharacter::GetIsSideClimbing()
{
	return bIsLadderSideClimbing;
}

float AMainCharacter::GetAxisClimbing()
{
	//return AxisClimbing * LadderSpeed;
	return AxisClimbingDir * LadderSpeed;
}

void AMainCharacter::ClimbLadder(float DeltaTime)
{
	if (bIsOnLadder && !bIsOnLadderTop && !bIsLadderSideClimbing)
	{
		float Gap = Cast<ALadder>(OwnLadder)->GetBarGap();

		if (AxisClimbing != 0 && !bIsLadderClimbing)
		{
			bIsLadderClimbing = true;
			AxisClimbingDir = AxisClimbing;
		}

		if (bIsLadderClimbing)
		{
			if (DeltaLadderClimbing < 2.0/* anim length */ / LadderSpeed)
			{
				float DeltaValue = Gap * AxisClimbingDir * LadderSpeed * DeltaTime;

				DeltaLadderClimbing += DeltaTime;
				this->SetActorLocation(this->GetActorLocation() + FVector(0, 0, DeltaValue));
			}
			else
			{
				bIsLadderClimbing = false;
				AxisClimbingDir = 0;
				DeltaLadderClimbing = 0;
			}
		}
	}
}

void AMainCharacter::ClimbLadderTop()
{
	if (AxisClimbing != 0 && bIsOnLadder && !bIsOnLadderTop)
	{
		bIsOnLadderTop = true;

		FVector StartLoc = GetActorLocation() + GetActorRotation().Vector() * 30 + FVector(0.0f, 0.0f, 110.0f);
		FVector EndLoc = StartLoc + GetActorRotation().Vector() * 100;
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(this);
		FHitResult OutHit;

		bool hit = UKismetSystemLibrary::SphereTraceSingle
		(
			GetWorld(),
			StartLoc,
			EndLoc,
			5.0,
			TraceTypeQuery1,
			true,
			IgnoreActors,
			EDrawDebugTrace::None,
			OutHit,
			true
		);

		float AboveWallDist = FVector::DotProduct((OutHit.Location - WallLoc), GetActorRotation().Vector());
		if (true)
		{
			if (!hit || AboveWallDist >= 60.0f)
			{
				PlayAnimMontage(M_ClimbLedge, 1.0f, NAME_None);
				bIsControllable = false;
			}
			else if (hit && AboveWallDist < 60.0f)
			{
				PlayAnimMontage(M_ClimbResistLedge, 1.0f, NAME_None);
				bIsControllable = false;
			}
		}
	}
}

void AMainCharacter::EndLadderTop()
{
	if (/*bIsOnLadder && */bIsOnLadderTop && GetCurrentMontage() != M_ClimbLedge)
	{
		// control?

		bIsOnLadder = false;
		bIsOnLadderTop = false;

		CharacterMovementComponent->SetMovementMode(MOVE_Walking);
		TriggerCapsuleComponent->SetCapsuleRadius(CapsuleDefaultRadius);

		bIsControllable = true;
	}
}

void AMainCharacter::ClimbLadderSide(float DeltaTime)
{
	if (bIsOnLadder && !bIsOnLadderTop && !bIsLadderClimbing)
	{
		float Gap = Cast<ALadder>(OwnLadder)->GetBarGap();

		FVector LLoc = OwnLadder->GetActorLocation();
		FVector LFwVec = OwnLadder->GetActorForwardVector();
		FVector LRVec = OwnLadder->GetActorRightVector();

		FVector CLoc = this->GetActorLocation();

		if (AxisSideClimbing != 0 && !bIsLadderSideClimbing)
		{
			// add condition
			// whether space is enough

			FVector LScale = OwnLadder->GetActorScale() * 0.8f /* hitbox size */;

			FVector VL = LLoc - LRVec * (LScale.Y * 50 - 20 /* half of character width */) - (CLoc + LFwVec * 15.f);
			FVector VR = LLoc + LRVec * (LScale.Y * 50 - 20 /* half of character width */) - (CLoc + LFwVec * 15.f);

			//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, FString::Printf(TEXT("LLoc (%.2f, %.2f, %.2f)"), LLoc.X, LLoc.Y, LLoc.Z));
			//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("CLoc + Offset (%.2f, %.2f, %.2f)"), (CLoc + LFwVec * 15.f).X, (CLoc + LFwVec * 15.f).Y, (CLoc + LFwVec * 15.f).Z));


			float DistRemL = sqrt(VL.X * VL.X + VL.Y * VL.Y);
			float DistRemR = sqrt(VR.X * VR.X + VR.Y * VR.Y);

			//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, FString::Printf(TEXT("Dist::%f %f"), DistRem, LadderSideSpeed * 1.166667));
			//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, FString::Printf(TEXT("Comp::%f + %f - %f"), LLoc.Y, (LRVec * (LScale.Y * 50 - 25)).Y, (CLoc + LFwVec * 15.f).Y));

			float MoveDistanceUnit = LadderSideSpeed * 1.166667; // move distance per 1cycle
			if (AxisSideClimbing < 0 && DistRemL > MoveDistanceUnit || AxisSideClimbing > 0 && DistRemR > MoveDistanceUnit) {
				bIsLadderSideClimbing = true;
				AxisClimbingDir = AxisSideClimbing;
			}
		}



		if (bIsLadderSideClimbing)
		{
			if (DeltaLadderSideClimbing < 1.166667/* anim length */)
			{
				float DeltaValue = AxisClimbingDir * LadderSideSpeed * DeltaTime;

				DeltaLadderSideClimbing += DeltaTime;

				this->SetActorLocation(CLoc + LRVec * DeltaValue);
			}
			else
			{
				bIsLadderSideClimbing = false;
				AxisClimbingDir = 0;
				DeltaLadderSideClimbing = 0;
			}
		}
	}
}

void AMainCharacter::CountTimeToEnableGrabLadder(float DeltaTime)
{
	if (TimeToEnableGrabLadder > 0.0f)
		TimeToEnableGrabLadder -= DeltaTime;
	else if (TimeToEnableGrabLadder < 0.0f)
		TimeToEnableGrabLadder = 0.0f;
}

// mj - Pulley
void AMainCharacter::SetPulley(AActor* A, bool b)
{
	bIsOnPulley = b;

	if (b)
	{
		APulley* P = Cast<APulley>(A);
		if (P)
		{
			MyPulley = A;
		}
	}
	else if (!bIsGrabbingPulley)
	{
		GrabPulley(false);
	}
}

void AMainCharacter::GrabPulley(bool b)
{
	if (b)
	{
		if (b == bIsPullingPulley)
			return;

		if (bIsOnPulley)
		{

			if (bIsFP)
			{
				if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 6, FColor::Cyan, FString::Printf(TEXT("switch cam to tp:%d"), bCantSwitchView));
				SwitchCamera();
			}
			TurnOffFlashLight();
			bCantSwitchView = true;

			bIsGrabbingPulley = true;
			bIsPullingPulley = true;

			CharacterMovementComponent->SetMovementMode(MOVE_Flying);
			CharacterMovementComponent->StopMovementImmediately();

			Cast<APulley>(MyPulley)->BeginPull();
			// animation
			// ik?
		}
	}
	else
	{
		if (b == bIsPullingPulley)
			return;

		if (bIsGrabbingPulley)
		{
			//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 6, FColor::Cyan, TEXT("disable grab pulley "));
			bIsGrabbingPulley = false;
			bIsPullingPulley = false;

			bCantSwitchView = false;

			CharacterMovementComponent->SetMovementMode(MOVE_Walking);


			Cast<APulley>(MyPulley)->BeginRelease();
			// disable ik?
		}
	}
}

void AMainCharacter::KeepGrabbingPulley()
{
	if (bIsGrabbingPulley)
	{
		// SetActorLocation(Cast<APulley>(MyPulley)->GetHandleLocation());

		FLatentActionInfo Info;
		Info.CallbackTarget = this;
		UKismetSystemLibrary::MoveComponentTo
		(
			TriggerCapsuleComponent,
			Cast<APulley>(MyPulley)->GetHandleLocation(),
			TriggerCapsuleComponent->GetRelativeRotation(),
			false,
			false,
			0.01,
			false,
			EMoveComponentAction::Move,
			Info
		);
	}
}

bool AMainCharacter::GetIsGrabbingPulley()
{
	return bIsGrabbingPulley;
}

void AMainCharacter::StartPullBoard()
{
	FVector Start;
	FVector End;
	FHitResult OutHit;
	if (bIsFP)
	{
		Start = FirstPersonCameraComponent->GetComponentLocation();
		End = Start + FirstPersonCameraComponent->GetForwardVector() * 100;
	}
	else
	{
		Start = TriggerCapsuleComponent->GetComponentLocation();
		End = Start + (TriggerCapsuleComponent->GetForwardVector() * 100);
	}

	TArray<AActor*> IgnoreActors;
	bool bIsInteracting = UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery1, false, IgnoreActors, EDrawDebugTrace::None,
		OutHit, true);
	if (bIsInteracting)
	{
		SlicableMesh = Cast<AMaster_Slicable>(OutHit.Actor);
		if (SlicableMesh && !bIsFP && !bIsFalling)
		{
			if (!SlicableMesh->bIsUsed)
			{
				bIsPullingBoard = true;
			}
		}
	}
}

void AMainCharacter::PullBoard()
{
	if (!(XAxisValue == 0.0f && YAxisValue == 0.0f) && CanPushOrPullDegree() && bIsOverlapWithBoard)
	{
		if (SlicableMesh)
		{
			FVector CFV = TriggerCapsuleComponent->GetForwardVector();
			if (XAxisValue < -0.9f || 0.9f < XAxisValue)
			{
				PullBoardChargingGauge(CFV.X, -XAxisValue, PullBoardMaxGauge);
			}
			else if (YAxisValue < -0.9f || 0.9f < YAxisValue)
			{
				PullBoardChargingGauge(CFV.Y, -YAxisValue, PullBoardMaxGauge);
			}
		}
	}
	else if (bIsPlayedPullBoardAnim)
	{
		PullBoardGauge = 0.0f;
		StopAnimMontage();
		SoundManager->StopPullBoardSound();
	}
}

void AMainCharacter::PullBoardChargingGauge(float ForwardVector, float AxisValue, float GaugeValue)
{
	bool bIsRightDirection = UKismetMathLibrary::NearlyEqual_FloatFloat(ForwardVector, AxisValue, 0.1f);
	if (bIsRightDirection)
	{
		if (!bIsPlayedPullBoardAnim && M_PullBoard)
		{
			bIsPlayedPullBoardAnim = true;
			if (SoundManager)
				SoundManager->PlayPullBoardSound(SlicableMesh->GetActorLocation());
			PlayAnimMontage(M_PullBoard);
		}
		PullBoardGauge = UKismetMathLibrary::FClamp(PullBoardGauge + 0.01, 0.0f, GaugeValue);
		if (UKismetMathLibrary::NearlyEqual_FloatFloat(PullBoardGauge, GaugeValue, 0.01f))
		{
			bIsPullBoardCharged = true;
		}
	}
}

void AMainCharacter::EndPullBoard()
{
	if (bIsAttachedAtPullBoard && !(GetCurrentMontage() ==  M_FallingDownToStand) && !(GetCurrentMontage() == M_PullBoardFallDown))
	{
		StopAnimMontage();
		TriggerCapsuleComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		TriggerCapsuleComponent->SetCapsuleSize(CapsuleDefaultRadius, CapsuleDefaultHeight, true);
		bIsPullingBoard = false;
		CharacterMovementComponent->bOrientRotationToMovement = true;
		SlicableMesh = nullptr;
		PullBoardGauge = 0.0f;
		bIsPullBoardCharged = false;
		bIsPlayedFallingAnim = false;
		bIsControllable = true;
		bIsAttachedAtPullBoard = false;
		bIsLeftHandHitWall = false;
		bIsRightHandHitWall = false;

		if (SoundManager)
		{
			SoundManager->StopPullBoardSound();
		}
	}
}


void AMainCharacter::PullBoardManager()
{
	if (bIsPullBoardCharged && !bIsPlayedFallingAnim)
	{
		bIsPlayedFallingAnim = true;
		FVector CharacterForwardVector = TriggerCapsuleComponent->GetForwardVector();
		bIsControllable = false;

		TriggerCapsuleComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		TriggerCapsuleComponent->SetCapsuleSize(CapsuleDefaultRadius * 0.8, CapsuleDefaultHeight * 0.8, true);
		SlicableMesh->SliceMesh(CharacterForwardVector);
		if (M_PullBoardFallDown)
		{
			if (SoundManager)
			{
				SoundManager->StopPullBoardSound();
				SoundManager->PlayWoodBrokenSound(SlicableMesh->GetActorLocation());
			}
			PlayAnimMontage(M_PullBoardFallDown);
			//FallingDownCurveTimeline->Play();
			if (SlicableMesh)
			{
				SlicableMesh->SlicableMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel8, ECollisionResponse::ECR_Block);
				SlicableMesh->OutHalfMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel8, ECollisionResponse::ECR_Block);
			}
		}
	}
	else if (bIsAttachedAtPullBoard)
	{
		if (!bIsPullBoardCharged)
			PullBoard();
		ForwardHandIK();
	}
}

void AMainCharacter::PressLMBForPullBoard()
{
	if (bIsDefaultState)
	{
		FVector StartLoc = TriggerCapsuleComponent->GetRelativeLocation();
		FVector EndLoc = TriggerCapsuleComponent->GetRelativeLocation() + GetActorForwardVector() * 40;
		TArray<AActor*> ToIgnore;
		FHitResult OutHit;
		bool bIsOutHit = UKismetSystemLibrary::SphereTraceSingle(
			GetWorld(),
			StartLoc,
			EndLoc,
			5.0f,
			ETraceTypeQuery::TraceTypeQuery8,
			false,
			ToIgnore,
			EDrawDebugTrace::None,
			OutHit,
			true
		);
		if (bIsOutHit)
		{
			SlicableMesh = Cast<AMaster_Slicable>(OutHit.Actor);
			if (!bIsAttachedAtPullBoard && SlicableMesh)
			{
				if (bIsFP)
					SwitchCamera();
				TurnOffFlashLight();
				CharacterMovementComponent->bOrientRotationToMovement = false;
				bIsAttachedAtPullBoard = true;

				FLatentActionInfo Info;
				Info.CallbackTarget = this;
				Info.ExecutionFunction = "PullBoardMoveComponentToFunc";
				Info.Linkage = 0;
				UKismetSystemLibrary::MoveComponentTo
				(
					TriggerCapsuleComponent,
					TriggerCapsuleComponent->GetRelativeLocation() + OutHit.Normal * 10,
					TriggerCapsuleComponent->GetRelativeRotation(),
					true,
					true,
					0.2,
					false,
					EMoveComponentAction::Move,
					Info
				);
			}
		}
	}
}

void AMainCharacter::PullBoardMoveComponentToFunc()
{
	if (bIsAttachedAtPullBoard)
	{
		TriggerCapsuleComponent->AttachToComponent(SlicableMesh->ProceduralMesh, FAttachmentTransformRules::KeepWorldTransform);
	}
}

void AMainCharacter::ReleaseLMBForPullBoard()
{
	EndPullBoard();
}

void AMainCharacter::SetMouseSensitivity(float ChangedValue)
{
	MouseSensitivityValue = ChangedValue;
}

void AMainCharacter::EscapeFromHidableObject()
{
	if (HidableObject)
	{
		HidableObject->Interact();
	}
}

void AMainCharacter::SetHidableObject(AInteractableObject_Hidable* Obj)
{
	HidableObject = Obj;
}


void AMainCharacter::SetIsFastFocusActorHidden(bool b)
{
	bIsFastFocusActorHidden = b;
	if(!bIsFP)
		SetVisibilityHidedActors(!b, FastFocusHidedActors);
}


void AMainCharacter::ForceToFPCam()
{
	if (!bIsFP)
	{
		bCantSwitchView = true;
		SwitchCamera();
	}
}


// numberLock
void AMainCharacter::ToggleLockInspector(bool b)
{
	bCantSwitchView = b;

	if (b)
	{
		bIsInspectingLock = true;

		// Prevent Fucking Cursor Escaping
		UGameplayStatics::SetViewportMouseCaptureMode(GetWorld(), EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown);

		PlayerController->bShowMouseCursor = true;
		PlayerController->GetHUD()->bShowHUD = false;


		PlayerController->SetIgnoreLookInput(true);
		PlayerController->SetIgnoreMoveInput(true);



		// set new transform
		FVector NewScale(.15f, .15f, .15f);
		FVector NewLocation = FirstPersonCameraComponent->GetComponentLocation() + FirstPersonCameraComponent->GetForwardVector() * 40.f;
		FRotator NewRotator = FirstPersonCameraComponent->GetComponentRotation();
		FTransform NewTransform(NewRotator, NewLocation, NewScale);

		Cast<ANumberLock>(NumberLock)->SetMainCharacter(this);
		NumberLock->SetActorTransform(NewTransform);
	}
	else
	{
		bIsInspectingLock = false;

		PlayerController->bShowMouseCursor = false;
		PlayerController->GetHUD()->bShowHUD = true;

		PlayerController->SetIgnoreLookInput(false);
		PlayerController->SetIgnoreMoveInput(false);

		// load transform
		Cast<ANumberLock>(NumberLock)->ResetTransform();


		LockWheel = 0;
		bIsClickingWheel = false;
	}
}

void AMainCharacter::ClickLockWheel(bool b)
{
	if (bIsInspectingLock)
	{

		if (b)	// press
		{
			FHitResult Out;

			bool bIsHit = PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, Out);
			if (bIsHit)
			{
				FString ComponentName = Cast<UStaticMeshComponent>(Out.GetComponent())->GetName();

				if (ComponentName.Contains("NumWheel"))
				{
					FString LStr;
					FString RStr;
					ComponentName.Split("NumWheel", &LStr, &RStr);
					LockWheel = FCString::Atoi(*RStr);


					bIsClickingWheel = true;

					float X;
					PlayerController->GetMousePosition(X, TmpMousePositionY);
				}

			}
		}
		else	//release
		{
			bIsClickingWheel = false;

			Cast<ANumberLock>(NumberLock)->Snap(LockWheel);

			LockWheel = 0;
		}
	}
}

void AMainCharacter::RollLockWheel(float DeltaTime)
{
	if (bIsClickingWheel)
	{
		float X, Y;
		PlayerController->GetMousePosition(X, Y);

		float offset = (Y - TmpMousePositionY) * DeltaTime * 10;
		if(abs(offset) > 1)
		{
			TmpMousePositionY += offset;
			Cast<ANumberLock>(NumberLock)->Roll(LockWheel, offset * -1);
		}

	}
}

// puzzle
void AMainCharacter::TogglePuzzleInspector(bool b) 
{

	bCantSwitchView = b;

	if (b)
	{
		bIsInspectingPuzzle = true;

		// Prevent Cursor Escaping
		UGameplayStatics::SetViewportMouseCaptureMode(GetWorld(), EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown);

		PlayerController->bShowMouseCursor = true;
		PlayerController->GetHUD()->bShowHUD = false;


		PlayerController->SetIgnoreLookInput(true);
		PlayerController->SetIgnoreMoveInput(true);



		// set new transform
		FVector NewScale(.2f, .2f, .2f);
		FVector NewLocation = FirstPersonCameraComponent->GetComponentLocation() + FirstPersonCameraComponent->GetForwardVector() * 40.f;
		FRotator NewRotator = FirstPersonCameraComponent->GetComponentRotation();
		FTransform NewTransform(NewRotator, NewLocation, NewScale);

		MPuzzle->SetActorTransform(NewTransform);
	}
	else
	{
		bIsInspectingPuzzle = false;

		PlayerController->bShowMouseCursor = false;
		PlayerController->GetHUD()->bShowHUD = true;

		PlayerController->SetIgnoreLookInput(false);
		PlayerController->SetIgnoreMoveInput(false);

		// load transform
		Cast<AMachineRoomPuzzle>(MPuzzle)->ResetTransform();
	}
}

void AMainCharacter::ClickPuzzleButton(bool b) 
{
	if (bIsInspectingPuzzle)
	{
		if (b)	// press
		{
			FHitResult Out;

			bool bIsHit = PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, Out);
			if (bIsHit)
			{
				FString ComponentName = Cast<UStaticMeshComponent>(Out.GetComponent())->GetName();

				if (ComponentName.Contains("Btn"))
				{
					FString LStr;
					FString RStr;

					ComponentName.Split("Btn", &LStr, &RStr);
					int n = FCString::Atoi(*RStr);

					Cast<AMachineRoomPuzzle>(MPuzzle)->Press(n);
				}
			}
		}
	}
}