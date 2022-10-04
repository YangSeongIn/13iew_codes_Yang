/*
* Header "MainCharacterAnimInstance"
* Created by Yang
* Last Modified by Yang
* Description:
* Main Character
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
// by Yang
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

// by mj
#include "Ladder.h"

#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "AI_Tags.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Sound/SoundCue.h"
#include "AI_Bot_Controller.h"

#include "MainCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDie);

UCLASS()
class CAP2_API AMainCharacter : public ACharacter, public IInterface_MainCharacterInfo
{
	GENERATED_BODY()

public:
	AMainCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Basic by Yang
	bool bIsDefaultState = false;
	bool bIsDefaultStateNotIncludingCrouching = false;
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
	bool bIsRunningJumping = false;
	bool bPressJumpKey = false;
	bool bIsFlashMode = false;
	bool bCanStand = false;
	bool bIsDead = false;
	bool bIsStartingPoint = true;
	bool bCantSwitchView = true;
	bool bHaveFlashLight = false;
	bool bIsWalkingOnSwimming = false;
	bool bIsOverlapWithBoard = false;
	bool bIsFallingDownPullBoard = false;
	bool bIsHoldingThrowable = false;
	bool bIsOnFocusHighlightable = false;
	bool bIsActivatingValve = false;

	const float CrouchHeight = 50.0f;
	const float CapsuleDefaultHeight = 62.0f;
	const float CapsuleDefaultRadius = 30.0f;
	float TurnRate = 0.0f;
	float LookUpRate = 0.0f;
	float TargetFocusDistance = 0.0f;
	float XAxisValue = 0.0f;
	float YAxisValue = 0.0f;
	float PoleRadValue = 0.0f;
	float MouseSensitivityValue = 1.0f;
	float MouseSensitivityWeight = 0.5f;

	// Rope by Yang
	bool bIsStaticRope = false;

	// Throw by Yang
	bool bIsThrowKeyDown = false;
	bool bReleasedLMB = false;
	bool bCanThrow = false;
	bool bIsPickingUp = false
	bool bIsGrabbingThrowable = false;
	float ThrowGauge = 0.0f;

	// PushPull by Yang
	bool bDoOncePushPull = false;
	bool bIsAttachedAtMovable = false;
	bool bIsLeftHandHitWall = false;
	bool bIsRightHandHitWall = false;

	// Ledge by Yang
	bool bIsOnLedge = false;
	bool bIsResisted = false;
	bool bDoOnceLedge = true;
	bool bCanLedgeRightHand = false;
	bool bCanLedgeLeftHand = false;
	float Range = 0.0f;
	FVector HeightLoc;
	FVector WallLoc;
	FVector WallNorm;
	FVector MovableObjNorm;
	FVector ClimbResistLocation;
	FRotator ClimbResistRotation;
	FRotator CameraSwitchRot;

	// LedgMove by Yang
	bool bIsLedgeMoveRight = false;
	bool bIsLedgeMoveLeft = false;
	FVector GoalLocLeft;
	FVector GoalLocRight;

	// Movable by Yang
	FVector IKLeftHandLocation;
	FVector IKRightHandLocation;

	void SetupStimulus();

public:
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
	UPROPERTY(EditAnywhere, Category = FlashLight)
		class AMaster_Slicable* SlicableMesh;
	UPROPERTY(EditAnywhere, Category = FlashLight)
		class UAIPerceptionStimuliSourceComponent* stimulus;
	UPROPERTY()
		USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY()
		class AMaster_Rope* _Rope;
	UPROPERTY()
		class AMaster_Pole* _Pole;

	// by Kang
	bool bIsHiding = false;

	//AI by Kang
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GamePlay")
		bool bIsPlayerExit_1stFloor = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GamePlay")
		bool bIsPlayerExit_2ndFloor = false;
	bool CanStand();
	// by Yang
	UPROPERTY(BlueprintReadWrite)
		float CrawlingSpeed = 0.0f;
	UPROPERTY(BlueprintReadWrite)
		float FallingDownSpeed = 0.0f;
	UPROPERTY(EditAnywhere, Category = "GamePlay")
		bool bIsTestingMode = false;
	UPROPERTY(EditAnywhere, Category = "Movement")
		float DefaultWalkSpeed = 200.0f;
	UPROPERTY(EditAnywhere, Category = "Movement")
		float DefaultCrouchSpeed = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Movement")
		float DefaultSprintSpeed = 400.0f;
	UPROPERTY(EditAnywhere, Category = "Movement")
		float DefaultJumpVelocity = 400.0f;
	UPROPERTY()
		APlayerController* PlayerController;
	UPROPERTY()
		FVector MainCharacterLocation;

	// Input Function by Yang
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
	void GetUpWhenRespawn();

	// Normal Function by Yang
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

	UPROPERTY()
		class AMaster_InteractableObject* InteractableObject;

	UPROPERTY(BlueprintReadWrite)
		TArray<AActor*> HidedActors;
	UFUNCTION(BlueprintCallable)
		void SetVisibilityHidedActors(bool b, TArray<AActor*> Actors);
	UFUNCTION()
		void ResetHidedActors();

	// AnimMontage
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_PullBoard;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_PullBoardFallDown;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_FallingDownToStand;
	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* M_HardLanding;
	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* M_CrashStop;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_ClimbLedge;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_ClimbResistLedge;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_LedgeMoveRight;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_LedgeMoveLeft;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_PickUp;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_ReadyToThrow;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_Throw;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_Slide;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_CrawlingCorpse;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_CrawlToStand;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_RespawnStand;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_FallFlat;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Animation")
		UAnimMontage* M_FallFront;

	// FirstPerson Camera - Focusing and Hud Image Constrol by Yang
	void DOF();
	void DOFSetting();
	void LineTraceHud();

	// Throw by Yang
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

	// Save and Load by Yang
	UFUNCTION(BlueprintCallable)
		void Die();
	void SaveLoadSetting();
	FOnDie OnDie;

	// Push Pull by Yang
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

	// Ledge by Yang & Kang
	bool bReleasedHangingLedge;
	void LedgeManager();
	void ForwardTrace(FVector StartLocation);
	void HeightTrace(float offset, bool* bCanLedgeHand);
	void GrabLedge();
	void ClimbUp();
	void ResetLedge();
	void StopHang();

	// Move On Ledge by Yang
	void CheckMoveLedge(FString Key);
	bool MoveLedgeHeightTrace(float offset, FString Key);
	bool MoveLedgeForwardTrace(FVector StartLocation, FString Key);
	void MoveLedgeManager();

	//Make Noise by Kang
	void MakeNoise(USoundCue* FootStep, FVector Location);

	// Get or Set Private Variables by Yang
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
	bool IsDead() { return bIsDead; };
	void SetIsOverlapWithBoard(bool b) { bIsOverlapWithBoard = b; };
	bool GetIsWalkingOnSwimming() { return bIsWalkingOnSwimming; };
	void SetIsActivatingValve(bool b) { bIsActivatingValve = b; };
	bool GetIsOnFocusHighlightable() { return bIsOnFocusHighlightable; };
	void SetIsHoldingThrowable(bool b) { bIsHoldingThrowable = b; };
	void SetHaveFlashLight(bool b) { bHaveFlashLight = b; };
	void SetIsStartingPoint(bool b) { bIsStartingPoint = b; };
	TTuple<FVector, bool> GetMovableLeftArmIKInfo() { return MakeTuple(IKLeftHandLocation, bIsLeftHandHitWall); };
	TTuple<FVector, bool> GetMovableRightArmIKInfo() { return MakeTuple(IKRightHandLocation, bIsRightHandHitWall); };
	TTuple<FVector, bool> GetPullBoardLeftArmIKInfo() { return MakeTuple(IKLeftHandLocation, bIsLeftHandPullBoard); };
	TTuple<FVector, bool> GetPullBoardRightArmIKInfo() { return MakeTuple(IKRightHandLocation, bIsRightHandPullBoard); };

	// Save and Load by Yang
	UPROPERTY()
		class UMyGameInstance* GameInstance;
	UFUNCTION()
		void Save();
	UFUNCTION()
		void Reset();

	// Character Input Control by Park
	void EnableInputCharacter();
	void DisableInputCharacter();
	void SetMouseSensitivity(float ChangedValue);

	// Pull Board by Park & Yang
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

	// DOF by Yang
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		float FocusDistance = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		float PowerDOF = 1.5f;
	FTimerHandle DOFTimerHandle;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
		FPostProcessSettings PPS;
	UPROPERTY()
		class UMaterialInstanceConstant* FPCameraWaterEffect;

	UPROPERTY(EditAnywhere)
		TArray<class AMaster_InteractableItem*> Inventory;

	// Timeline by Yang
	FOnTimelineFloat SmoothCrouchInterpFunction;
	FOnTimelineEvent SmoothCrouchTimelineFinish;
	UFUNCTION()
		void SmoothCrouchInterpReturn(float Value);
	UPROPERTY()
		UTimelineComponent* SmoothCrouchCurveTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
		UCurveFloat* SmoothCrouchCurveFloat;

	// Movable by Yang
	UPROPERTY()
		class AMaster_Movable* MovableObj;

	class AMaster_Throwable* ThrowableObject;

	// CameraShake by Yang
	UPROPERTY(EditAnywhere, Category = "CameraShake")
		TSubclassOf<UCameraShakeBase> CameraShake_Walk;
	UPROPERTY(EditAnywhere, Category = "CameraShake")
		TSubclassOf<UCameraShakeBase> CameraShake_Sprint;

	// Widget by Yang
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

	UPROPERTY(EditAnywhere)
		class ASoundManager* SoundManager;

	// Hide by Park
	UPROPERTY()
		class AInteractableObject_Hidable* HidableObject;
	void EscapeFromHidableObject();
	void SetHidableObject(class AInteractableObject_Hidable* Obj);

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
	float AxisClimbing;
	float AxisSideClimbing;
	float AxisClimbingDir;

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
	UPROPERTY()
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
	UPROPERTY()
		AActor* NumberLock;

public:
	void ToggleLockInspector(bool b);
	void ClickLockWheel(bool b);
	void RollLockWheel(float DeltaTime);

	//----------------
	// puzzle
private:
	bool bIsInspectingPuzzle = false;
	UPROPERTY()
		AActor* MPuzzle;

public:
	void TogglePuzzleInspector(bool b);
	void ClickPuzzleButton(bool b);


};
