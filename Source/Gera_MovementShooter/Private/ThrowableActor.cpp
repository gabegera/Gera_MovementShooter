// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowableActor.h"

// Sets default values
AThrowableActor::AThrowableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	StaticMesh->SetupAttachment(RootComponent);	
}

// Called when the game starts or when spawned
void AThrowableActor::BeginPlay()
{
	Super::BeginPlay();
}
	


void AThrowableActor::StartFuseTimer(float FuseTime)
{
	GetWorldTimerManager().SetTimer(FuseTimerHandle, this, &AThrowableActor::Explode, FuseTime, false);
}

void AThrowableActor::Explode()
{
	TArray<FHitResult> HitArray;
	
	TArray<AActor*> ActorsToIgnore;
	
	UGameplayStatics::ApplyRadialDamageWithFalloff(this, Damage, 0, GetActorLocation(), BlastRadius, BlastRadius, 0, nullptr, ActorsToIgnore, this, nullptr);
	bool isHit = UKismetSystemLibrary::SphereTraceMultiByProfile(this, GetActorLocation(), GetActorLocation(), BlastRadius, FName("PhysicsActor"), false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitArray, true, FLinearColor::Green, FLinearColor::Red, 1.0f);

	if (isHit)
	{
		for (auto& Hit : HitArray)
		{
			if (Hit.GetComponent()->GetClass() == UStaticMeshComponent::StaticClass())
			{
				Hit.GetComponent()->AddRadialImpulse(GetActorLocation(), BlastRadius, Damage * 1000.0f, RIF_Constant, false);
			}
		}		
	}


	if (GrenadeExplosionParticle != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, GrenadeExplosionParticle, GetActorLocation(), FRotator::ZeroRotator, true);
	}
	Destroy();
}

// Called every frame
void AThrowableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

