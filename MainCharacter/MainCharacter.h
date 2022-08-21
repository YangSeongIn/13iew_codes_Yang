
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Interface_MainCharacterInfo.h"
#include "Components/PostProcessComponent.h"
#include "Interface_InteractWithWidget.h"
#include "Components/SpotLightComponent.h"

// mj
#include "Ladder.h"

#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "AI_Tags.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Sound/SoundCue.h"

#include "MainCharacter.generated.h"

DECLARE_DELEGATE(FOnDie);

UCLASS()
class CAP2_API AMainCharacter : public ACharacter, public IInterface_MainCharacterInfo
{
	GENERATED_BODY()

private:
	// Basic
	bool bIsDefaultState;
	bool bIsDefaultStateNotIncludingCrouching;
	bool bIsControllable = true;
	bool bIsSprinting = false;
	bool bIsFalling = false;
	bool bIsFP = false;
	bool bIsMoving = false;
	bool bIsCrouching = false;
	bool bIsReadyToLand = false;
	bool bIsPressLMB = false;
	bool bIsHangingRope = false;
	bool bIsPressingCrouchingKey = false;
	bool bIsSliding = false;
	bool bIsOnPole = false;
	bool bIsPressingSprintKey;
	bool bIsWithDoctorWhenDie = true;
	bool bIsRunningJumping;
	bool bPressJumpKey;
	bool bIsFlashMode;

	const float CrouchHeight = 50.0f;
	const float CapsuleDefaultHeight = 62.0f;
	const float CapsuleDefaultRadius = 30.0f;
	float TurnRate;
	float LookUpRate;
	float TargetFocusDistance = 0.0f;
	float XAxisValue = 0.0f;
	float YAxisValue = 0.0f;
	float PoleRadValue = 0.0f;
	float MouseSensitivityValue = 1.0f;
	float MouseSensitivityWeight = 0.5f;

	// Rope
	bool bIsStaticRope;

	// Throw
	bool bIsThrowKeyDown;
	bool bReleasedLMB;
	bool bCanThrow;
	bool bIsPickingUp ;
	bool bIsGrabbingThrowable ;
	float ThrowGauge = 0.0f;

	// PushPull
	bool bDoOncePushPull;
	bool bIsAttachedAtMovable;
	bool bIsLeftHandHitWall;
	bool bIsRightHandHitWall;

	// Ledge
	bool bIsOnLedge;
	bool bIsResisted;
	bool bDoOnceLedge = true;
	bool bCanLedgeRightHand;
	bool bCanLedgeLeftHand;
	float Range;
	FVector HeightLoc;
	FVector WallLoc;
	FVector WallNorm;
	FVector MovableObjNorm;
	FVector ClimbResistLocation;
	FRotator ClimbResistRotation;
	FRotator CameraSwitchRot;

	// LedgMove
	bool bIsLedgeMoveRight;
	bool bIsLedgeMoveLeft;
	FVector GoalLocLeft;
	FVector GoalLocRight;

	// Movable
	FVector IKLeftHandLocation;
	FVector IKRightHandLocation;

	// AI
	class UAIPerceptionStimuliSourceComponent* stimulus;
	void SetupStimulus();

public:
	AMainCharacter();

	bool bIsHiding = false;
	UPROPERTY(BlueprintReadWrite)
		bool bIsCrawlingAnim = false;
	bool bCantSwitchView = true;
	bool bHaveFlashLight;
	bool bIsWalkingOnSwimming;
	bool bIsOverlapWithBoard;
	bool bIsFallingDownPullBoard;
	bool bIsHoldingThrowable;
	bool bIsOnFocusHighlightable = false;
	bool bIsActivatingValve = false;
	bool bIsChangingDirection;
	bool bDoOnceChangingDirection = true;

	//UPROPERTY(EditAnywhere, Category = "GamePlay")
	bool bIsStartingPoint = true;
	UPROPERTY(BlueprintReadWrite)
		float CrawlingSpeed = 0.0f;
	UPROPERTY(BlueprintReadWrite)
		float FallingDownSpeed = 0.0f;
	UPROPERTY(EditAnywhere, Category = "GamePlay")
		bool bIsTestingMode;
	UPROPERTY(EditAnywhere, Category = "Movement")
		float DefaultWalkSpeed = 200.0f;
	UPROPERTY(EditAnywhere, Category = "Movement")
		float DefaultCrouchSpeed = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Movement")
		float DefaultSprintSpeed = 400.0f;
	UPROPERTY(EditAnywhere, Category = "Movement")
		float DefaultJumpVelocity = 400.0f;

	APlayerController* PlayerController;
	UPROPERTY()
		FVector MainCharacterLocation;
	UPROPERTY()
		USkeletalMeshComponent* SkeletalMesh;

	// Input Function
	void LookX(float AxisValue);
	void LookY(float AxisValue);
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void StartSprint();
	void StopSprint();
	void StartJump();
	void StopJump();
	void StartCrouch();
	void StopCrouch();
	void LeftMouseButtonPress();
	void LeftMouseButtonRelease();
	void Interact();
	void StopInteract();
	void SwitchCamera();
	void TestDie();
	void ESCMenu();
	void FlashLight();
	void LedgeMove();
	void OpenKeyExplanationWidget();
	void CheckEdgeSlip();

	// Normal Function
	void CharacterMaxWalkSpeedManager();
	void CrouchManager();
	void FallingManager();
	void TimelineSetting();
	void ReleaseRope();
	void LineTraceLMB();
	void LineTraceLMBReleased();
	void InitialSetting();
	void DefaultWidgetManager();
	void TurnOffFlashLight();
	void SwimmingManager();

	class AMaster_InteractableObject* InteractableObject;

	UPROPERTY(BlueprintReadWrite)
		TArray<AActor*> HidedActors;
	UFUNCTION(BlueprintCallable)
		void SetVisibilityHidedActors(bool b, TArray<AActor*> Actors);
	UFUNCTION()
		void ResetHidedActors();

	// FirstPerson Camera - Focusing and Hud Image Constrol 
	void DOF();
	void DOFSetting();
	void LineTraceHud();

	// Throw
	void ThrowManager();
	void PressLMBForThrow();
	void ReleaseLMBForThrow();
	void PressLMBForClimbingPole();
	void ReleaseLMBClimbingPole();
	void DropDown();
	void DelegateSetting();
	UFUNCTION()
		void OnMontageNotifyBegin();
	UFUNCTION()
		void OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted);
	UFUNCTION()
		void OnMontageBlendOut(UAnimMontage* AnimMontage, bool bInterrupted);
	FScriptDelegate Delegate_OnMontageNotifyBegin;

	// Save and Load
	UFUNCTION(BlueprintCallable)
		void Die();
	void SaveLoadSetting();
	FOnDie OnDie;

	// Push Pull
	TTuple<FVector, bool> PushPullHandIK(FName SocketName, float Distance);
	bool CanPushOrPullDegree();
	void MovableObjPushPull();
	bool GetIsPushOrPull();
	float PushOrPullRotate(bool bIsPush);
	float GetPushPullDir();
	void PushPullManager();
	void PressLMBForMovable();
	void ReleaseLMBForMovable();
	UFUNCTION()
		void MovableMoveComponentToFunc();

	// Ledge
	bool bReleasedHangingLedge;
	void LedgeManager();
	void ForwardTrace(FVector StartLocation);
	void HeightTrace(float offset, bool* bCanLedgeHand);
	void GrabLedge();
	void ClimbUp();
	void ResetLedge();
	void StopHang();

	// Move On Ledge
	void CheckMoveLedge(FString Key);
	bool MoveLedgeHeightTrace(float offset, FString Key);
	bool MoveLedgeForwardTrace(FVector StartLocation, FString Key);
	void MoveLedgeManager();


	//Make Noise
	void MakeNoise(USoundCue* FootStep, FVector Location);

	// Get or Set Private Variables
	bool GetIsFalling() { return bIsFalling; };
	bool GetIsCrouching() { return bIsCrouching; };
	bool GetIsReadyToLand() { return bIsReadyToLand; };
	bool GetIsRopeHanging() { return bIsHangingRope; };
	bool GetIsAttachedAtMovableObj() { return  bIsAttachedAtMovable; };
	float GetCapsuleComponentNormalHeight() { return CapsuleDefaultHeight; };
	bool GetIsStaticRope() { return bIsStaticRope; };
	UFUNCTION(BlueprintCallable)
		bool GetIsControllable() { return bIsControllable; };
	UFUNCTION(BlueprintCallable)
		void SetIsControllable(bool b) { bIsControllable = b; };
	bool GetIsGrabbingThrowableObject() { return bIsGrabbingThrowable; };
	bool GetIsPullingBoard() { return bIsPullingBoard; };
	void SetIsCrouching(bool b) { bIsCrouching = b; };
	bool GetIsOnPole() { return bIsOnPole; };
	float GetXAxisValue() { return XAxisValue; };
	float GetYAxisValue() { return YAxisValue; };
	bool GetIsFP() { return bIsFP; };
	void SetIsLeftHandHitWall(bool b) { bIsLeftHandHitWall = b; };
	void SetIsRightHandHitWall(bool b) { bIsRightHandHitWall = b; };
	void SetIsWithDoctorWhenDie(bool b) { bIsWithDoctorWhenDie = b; };
	bool GetIsWithDoctorWhenDie() { return bIsWithDoctorWhenDie; };
	bool GetIsRunningJumping() { return bIsRunningJumping; };
	bool GetIsPressJumpKey() { return bPressJumpKey; };
	bool HasFlashLight() { return bIsFlashMode; };
	TTuple<FVector, bool> GetMovableLeftArmIKInfo() { return MakeTuple(IKLeftHandLocation, bIsLeftHandHitWall); };
	TTuple<FVector, bool> GetMovableRightArmIKInfo() { return MakeTuple(IKRightHandLocation, bIsRightHandHitWall); };
	TTuple<FVector, bool> GetPullBoardLeftArmIKInfo() { return MakeTuple(IKLeftHandLocation, bIsLeftHandPullBoard); };
	TTuple<FVector, bool> GetPullBoardRightArmIKInfo() { return MakeTuple(IKRightHandLocation, bIsRightHandPullBoard); };

	// Save and Load
	class UMyGameInstance* GameInstance;
	UFUNCTION()
		void Save();
	UFUNCTION()
		void Reset();

	// Park, Character Input Control
	void EnableInputCharacter();
	void DisableInputCharacter();
	void SetMouseSensitivity(float ChangedValue);

	// Park, Pull Board
	//TTuple<FVector, bool> PullBoardHandIK(FName SocketName, float Distance);
	class AMaster_Slicable* SlicableMesh;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_PullBoard;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_PullBoardFallDown;
	void ForwardHandIK();

	void StartPullBoard();
	void PullBoard();
	void PullBoardChargingGauge(float ForwardVector, float AxisValue, float GaugeValue);
	void EndPullBoard();
	void PullBoardManager();
	void PressLMBForPullBoard();
	void ReleaseLMBForPullBoard();
	UFUNCTION()
		void PullBoardMoveComponentToFunc();

	bool bIsPullingBoard; // Delete
	bool bIsAttachedAtPullBoard;
	bool bIsPlayedPullBoardAnim;
	bool bIsPullBoardCharged;
	bool bIsPlayedFallingAnim;
	bool bIsLeftHandPullBoard;
	bool bIsRightHandPullBoard;
	float PullBoardGauge;
	UPROPERTY(EditAnyWhere, Category = "PullBoard")
		float PullBoardMaxGauge = 2.5f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_FallingDownToStand;

	// DOF
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		float FocusDistance = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		float PowerDOF = 1.5f;
	FTimerHandle DOFTimerHandle;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		FPostProcessSettings PPS;
	class UMaterialInstanceConstant* FPCameraWaterEffect;

	// Component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UCapsuleComponent* TriggerCapsuleComponent;
	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArmComponent;
	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* SideViewCameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		UCameraComponent* FirstPersonCameraComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCharacterMovementComponent* CharacterMovementComponent;
	UPROPERTY(EditAnywhere, Category = Camera)
		UPostProcessComponent* PostProcessComponent;
	UPROPERTY(EditAnywhere, Category = FlashLight)
		UStaticMeshComponent* FlashLight_SV;
	UPROPERTY(EditAnywhere, Category = FlashLight)
		UStaticMeshComponent* FlashLight_FP;
	UPROPERTY(EditAnywhere, Category = FlashLight)
		USpotLightComponent* SpotLight_SV;
	UPROPERTY(EditAnywhere, Category = FlashLight)
		USpotLightComponent* SpotLight_FP;

	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* M_HardLanding;
	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* M_CrashStop;
	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* M_EdgeSlip;
	UPROPERTY(EditAnywhere)
		TArray<class AMaster_InteractableItem*> Inventory;

	// OnCollisionEnter & End
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Timeline
	FOnTimelineFloat SmoothCrouchInterpFunction;
	FOnTimelineEvent SmoothCrouchTimelineFinish;
	UFUNCTION()
		void SmoothCrouchInterpReturn(float Value);
	UPROPERTY()
		UTimelineComponent* SmoothCrouchCurveTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
		UCurveFloat* SmoothCrouchCurveFloat;

	FOnTimelineFloat CrawlingInterpFunction;
	FOnTimelineEvent CrawlingTimelineFinish;
	UFUNCTION()
		void CrawlingInterpReturn(float Value);
	UPROPERTY()
		UTimelineComponent* CrawlingCurveTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
		UCurveFloat* CrawlingCurveFloat;

	FOnTimelineFloat FallingDownInterpFunction;
	UFUNCTION()
		void FallingDownInterpReturn(float Value);
	UPROPERTY()
		UTimelineComponent* FallingDownCurveTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
		UCurveFloat* FallingDownCurveFloat;

	// Rope
	class AMaster_Rope* _Rope;

	// Pole
	class AMaster_Pole* _Pole;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_JumpOnWall;

	// Ledge
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_ClimbLedge;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_ClimbResistLedge;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_LedgeMoveRight;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_LedgeMoveLeft;

	// Movable
	class AMaster_Movable* MovableObj;

	// Throw
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_PickUp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_ReadyToThrow;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_Throw;
	class AMaster_Throwable* ThrowableObject;

	// CameraShake
	UPROPERTY(EditAnywhere, Category = "CameraShake")
		TSubclassOf<UCameraShakeBase> CameraShake_Walk;
	UPROPERTY(EditAnywhere, Category = "CameraShake")
		TSubclassOf<UCameraShakeBase> CameraShake_Sprint;

	// Widget
	IInterface_InteractWithWidget* InterfaceWithWidget;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> _DefaultWidget;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Runtime")
		class UFirstPersonDefaultWidget* DefaultWidget;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> ESCMenuWidgetClass;
	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
		class UESCMenuWidget* ESCMenuWidget;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> _KeyExplanationWidget;
	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
		class UKeyExplanationWidget* KeyExplanationWidget;

	// Sliding
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_Slide;

	// CutScene
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_CrawlingCorpse;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_CrawlToStand;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_RespawnStand;

	// Death
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_FallFlat;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_FallFront;

	// ChangeDirection
	bool bCanChangeDirectionX;
	bool bCanChangeDirectionY;
	float ChangeDirectionTime;
	bool CanChangeDirection(float AxisValue, float Axis);
	void ChangeDirectionManager();
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_ChangeDirection;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_ChangeDirection_Mirror;

	UPROPERTY(EditAnywhere)
		class ASoundManager* SoundManager;

	// Hide
	class AInteractableObject_Hidable* HidableObject;
	void EscapeFromHidableObject();
	void SetHidableObject(class AInteractableObject_Hidable* Obj);
	// protected
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Interface Method
public:
	virtual bool IsMainCharacter() override { return IsPlayerControlled(); };
	virtual bool GetIsPressLMB() override { return bIsPressLMB; };
	virtual void GrapRope(class AMaster_Rope* Rope, float HangingLocZ) override;
	virtual bool GetIsHangingRope() override { return bIsHangingRope; };



	// MJ


	// Camera Switching
private:

public:
	//tmp
	void SetCameraToThirdPersonMode();
	void SetCameraUtility(AActor* CU);

	UPROPERTY(VisibleAnywhere)
		AActor* CameraUtility;


	// Ladder
private:
	bool bIsLadderReady;
	bool bIsOnLadder;
	bool bIsOnLadderTop;
	//bool bIsOnLadderTop;
	//float AxisInputF;
	float AxisClimbing;
	float AxisSideClimbing;
	float AxisClimbingDir;
	//float LHandIKAlpha;
	//float RHandIKAlpha;
	//FVector LHandIKEffector;
	//FVector RHandIKEffector;
	//int MAxisLadder;

	float LadderSpeed = 2.0f;
	float LadderSideSpeed = 40.0f;
	float DeltaLadderClimbing = 0;
	float DeltaLadderSideClimbing = 0;
	bool bIsLadderClimbing = false;
	bool bIsLadderSideClimbing = false;

	float HOffset = 64.15f;
	float TimeToEnableGrabLadder = 0;

public:
	void SetLadder(AActor* Ladder);
	void GrabLadder(bool b);
	bool GetIsOnLadder();
	bool GetIsClimbing();
	bool GetIsSideClimbing();
	float GetAxisClimbing();

	// vertical
	void ClimbLadder(float DeltaTime);
	void ClimbLadderTop();
	void EndLadderTop();
	// horizontal
	void ClimbLadderSide(float DeltaTime);

	void CountTimeToEnableGrabLadder(float DeltaTime);

	UPROPERTY(VisibleAnywhere)
		AActor* OwnLadder;


	// Pulley
private:
	AActor* MyPulley;
	bool bIsOnPulley;
	bool bIsGrabbingPulley;
	bool bIsPullingPulley;

public:
	//input
	void SetPulley(AActor* A, bool b);
	void GrabPulley(bool b);
	void KeepGrabbingPulley();
	bool GetIsGrabbingPulley();

	// Cam - Fastfocus
	bool bIsFastFocusActorHidden = false;
	void SetIsFastFocusActorHidden(bool b);
	UPROPERTY(BlueprintReadWrite)
		TArray<AActor*> FastFocusHidedActors;

	// - Demo
	void ForceToFPCam();

	//-------------
	// number lock
private:
	bool bIsInspectingLock = false;
	bool bIsClickingWheel = false;
	float TmpMousePositionY = 0;
	int LockWheel = 0;

	AActor* NumberLock;

public:
	void ToggleLockInspector(bool b);
	void ClickLockWheel(bool b);
	void RollLockWheel(float DeltaTime);
};
