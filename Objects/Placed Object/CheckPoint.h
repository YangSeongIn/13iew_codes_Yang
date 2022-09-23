/*
* Header "CheckPoint"
* Created by Yang
* Last Modified by Yang
* Description:
* Save / Load : When character overlaps on this object, save all data about current level in SaveGameObject.h.
* Next Level : When character overlaps on this object, open next level.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "MyEnums.h"

#include "CheckPoint.generated.h"

UCLASS()
class CAP2_API ACheckPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ACheckPoint();
	// box to next level or save game
	UPROPERTY(EditAnywhere)
		bool bIsNextLevel = false;
	// boolean to save the use of this object
	UPROPERTY()
		bool bIsDestroyed = false;
	// Indicates the name of the saved point in the main menu
	UPROPERTY(EditAnywhere)
		FString SavePointName;

	// components
	UPROPERTY(EditAnywhere)
		USceneComponent* RespawnLocation;
	UPROPERTY(EditAnywhere)
		UBoxComponent* Box;
	UPROPERTY(EditAnywhere)
		UArrowComponent* Arrow;

	UPROPERTY()
		class AMainCharacter* MainCharacter;
	UPROPERTY()
		class UMyGameInstance* GameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		TEnumAsByte<ECheckPointRespawnType> CheckPointRespawnType;

	UFUNCTION()
		void Reset();
	UFUNCTION()
		void Save();

	void SaveGameToSlot();
	void NormalCheckPoint();
	void ToNextLevel();


	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

// About Enum
public:
	// Make enum to string. (for getting name of level)
	template <typename Enumeration>
	static FORCEINLINE FString GetEnumerationToString(const Enumeration InValue)
	{
		return StaticEnum<Enumaeration>()->GetNameStringByValue(static_cast<int64>(InValue));
	}

protected:
	virtual void BeginPlay() override;
};
