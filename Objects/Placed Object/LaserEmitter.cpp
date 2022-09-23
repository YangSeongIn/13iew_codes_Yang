#include "LaserEmitter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInterface.h"
#include "Projector.h"
#include "MovieSceneSequencePlayer.h"

ALaserEmitter::ALaserEmitter()
{
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Glass"));
	RootComponent = StaticMesh;

	Cube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	Cube->SetRelativeScale3D(FVector(0.3f));
	Cube->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MirrorAsset(TEXT(
		"MaterialInstanceConstant'/Game/StarterContent/Materials/M_Metal_Burnished_Steel_Inst.M_Metal_Burnished_Steel_Inst'"));
	if (MirrorAsset.Succeeded())
	{
		MirrorMat = MirrorAsset.Object;
	}
	SetActorTickInterval(0.1f);
}

void ALaserEmitter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CastLight(Cube->GetComponentLocation(), Cube->GetForwardVector());
}

void ALaserEmitter::CastLight(FVector CastOrigin, FVector CastDirection)
{
	FVector _CastOrigin = CastOrigin + CastDirection * LaserStartPoint;
	FVector _CastDirection = CastDirection;
	int32 ReflectionNum = 0;
	bool bIsMirror = true;
	while (bIsMirror && MaxReflectionNum > ReflectionNum)
	{
		TArray<AActor*> ToIgnore;
		ToIgnore.Add(this);
		FHitResult OutHit;
		bool bIsOutHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), _CastOrigin, _CastOrigin + _CastDirection * 3000.0f, ETraceTypeQuery::TraceTypeQuery4, false,
			ToIgnore, EDrawDebugTrace::ForDuration, OutHit, true, FLinearColor::Red, FLinearColor::Green, 0.1f);

		// Laser Emitter
		if (bIsOutHit)
		{
			if (LaserEmitter)
			{
				UParticleSystemComponent* _LaserEmitter = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LaserEmitter, FVector(0.0f), FRotator(0.0f), FVector(1.0f), true, EPSCPoolMethod::None, true);
				_LaserEmitter->SetBeamSourcePoint(0, _CastOrigin, 0);
				_LaserEmitter->SetBeamEndPoint(0, OutHit.ImpactPoint);
			}
			
			
			if (OutHit.Actor->GetClass()->IsChildOf(AProjector::StaticClass()))
			{
				AProjector* Projector = Cast<AProjector>(OutHit.Actor);
				UStaticMeshComponent* StaticMesh1 = Cast<UStaticMeshComponent>(OutHit.Component);
				if (Projector && Projector->bDoOncePlay)
				{
					Projector->bDoOncePlay = false;
					Projector->ProjectVideo();
				}
				else
				{
					ReflectionNum = 0;
				}
				bIsMirror = false;
			}
			else
			{
				if (MirrorMat != nullptr && MirrorMat == OutHit.Component->GetMaterial(0))
				{
					_CastOrigin = OutHit.ImpactPoint + _CastDirection.MirrorByVector(OutHit.ImpactNormal);
					_CastDirection = _CastDirection.MirrorByVector(OutHit.ImpactNormal);
					bIsMirror = true;
					ReflectionNum++;
				}
				else
				{
					ReflectionNum = 0;
					bIsMirror = false;
				}
			}
		}
		else
		{
			if (LaserEmitter)
			{
				UParticleSystemComponent* _LaserEmitter = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LaserEmitter, FVector(0.0f), FRotator(0.0f), FVector(1.0f), true, EPSCPoolMethod::None, true);
				_LaserEmitter->SetBeamSourcePoint(0, _CastOrigin, 0);
				_LaserEmitter->SetBeamEndPoint(0, _CastOrigin + _CastDirection * 3000.0f);
			}
			ReflectionNum = 0;
			bIsMirror = false;
		}
	}
}