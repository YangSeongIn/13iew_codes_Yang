// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "MyEnums.h"

#include "CheckPoint.generated.h"

//UENUM(BlueprintType)
//enum class ECheckPointRespawnType : uint8
//{ 
//	None UMETA(DisplayName = "None"),
//	Crawling UMETA(DisplayName = "Crawling"),
//	Getup UMETA(DisplayName = "Getup"), 
//	WalkIn UMETA(DisplayName = "WalkIn")
//};

UCLASS()
class CAP2_API ACheckPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ACheckPoint();

	void SaveGameToSlot();
	void NormalCheckPoint();
	void ToNextLevel();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		TEnumAsByte<ECheckPointRespawnType> CheckPointRespawnType;

	UPROPERTY(EditAnywhere)
		bool bIsNextLevel = false;

	UPROPERTY()
		bool bIsDestroyed = false;

	UPROPERTY(EditAnywhere)
		FString SavePointName;

	UPROPERTY()
		class AMainCharacter* MainCharacter;

	UPROPERTY()
		class UMyGameInstance* GameInstance;
	UFUNCTION()
		void Reset();
	UFUNCTION()
		void Save();

	UPROPERTY(EditAnywhere)
		UBoxComponent* Box;
	UPROPERTY(EditAnywhere)
		UArrowComponent* Arrow;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// About Enum
public:
	template <typename Enumeration>
	static FORCEINLINE FString GetEnumerationToString(const Enumeration InValue)
	{
		return StaticEnum<Enumaeration>()->GetNameStringByValue(static_cast<int64>(InValue));
	}

protected:
	virtual void BeginPlay() override;
};
