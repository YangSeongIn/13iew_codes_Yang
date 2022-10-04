/*
* Header "Balance"
* Created by Yang
* Last Modified by Yang
* Description:
* Gimmick on 1st floor in balance room.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

#include "Balance.generated.h"

UCLASS()
class CAP2_API ABalance : public AActor
{
	GENERATED_BODY()
	
public:	
	ABalance();

	UPROPERTY(BlueprintReadWrite)
		float Plate1Mass;
	UPROPERTY(BlueprintReadWrite)
		float Plate2Mass;

	float CurrentArmRotation;

	UPROPERTY(EditAnywhere)
		float MainCharacterMass;
	UPROPERTY(EditAnywhere)
		float ThrowableMass;
	UPROPERTY(EditAnywhere)
		float MassGap;
	UPROPERTY()
		class AMainCharacter* MainCharacter;

	UPROPERTY(EditAnywhere)
		USceneComponent* DefaultSceneRoot;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Body;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Body_Child;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Arm;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Plate1;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Plate2;
	UPROPERTY(EditAnywhere)
		UBoxComponent* Box1;
	UPROPERTY(EditAnywhere)
		UBoxComponent* Box2;
	UPROPERTY(EditAnywhere)
		UPhysicsConstraintComponent* PhysicsConstraint1;
	UPROPERTY(EditAnywhere)
		UPhysicsConstraintComponent* PhysicsConstraint2;

	UFUNCTION(BlueprintCallable)
		float AddMassOnPlate(float Mass, AActor* OtherActor);
	UFUNCTION(BlueprintCallable)
		float SubMassOnPlate(float Mass, AActor* OtherActor);

	UFUNCTION()
		void OnBox1OverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnBox1OverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
		void OnBox2OverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnBox2OverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
