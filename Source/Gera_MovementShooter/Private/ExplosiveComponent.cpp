// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveComponent.h"

#include "Components/CapsuleComponent.h"


// Sets default values for this component's properties
UExplosiveComponent::UExplosiveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UExplosiveComponent::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->GetWorldTimerManager().SetTimerForNextTick(this, &UExplosiveComponent::StartFuseTimer);
	
}


// Called every frame
void UExplosiveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UExplosiveComponent::Explode()
{
	TArray<FHitResult> HitArray;
	
	TArray<AActor*> ActorsToIgnore;

	FVector ActorLocation = GetOwner()->GetActorLocation();
	UGameplayStatics::ApplyRadialDamageWithFalloff(this, Damage, 0, ActorLocation, BlastRadius, BlastRadius, 0, nullptr, ActorsToIgnore, GetOwner(), nullptr);
	bool isHit = UKismetSystemLibrary::SphereTraceMultiByProfile(this, ActorLocation, ActorLocation, BlastRadius, FName("PhysicsActor"), false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitArray, true, FLinearColor::Green, FLinearColor::Red, 1.0f);

	if (isHit)
	{
		for (auto& Hit : HitArray)
		{
			if (Hit.GetComponent()->GetClass() == UStaticMeshComponent::StaticClass())
			{
				Hit.GetComponent()->AddRadialImpulse(ActorLocation, BlastRadius, Damage * 1000.0f, RIF_Constant, false);
			}
			else if (Hit.GetComponent()->GetClass() == UCapsuleComponent::StaticClass())
			{
				Hit.GetComponent()->AddRadialImpulse(ActorLocation, BlastRadius, Damage * 1000.0f, RIF_Constant, false);
			}
		}
	}


	if (ExplosionEffect != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionEffect, ActorLocation, FRotator::ZeroRotator, true);
	}
	GetOwner()->Destroy();
}

void UExplosiveComponent::StartFuseTimer()
{
	if (BlastRadius > 0.0f)
	{
		GetOwner()->GetWorldTimerManager().SetTimer(FuseTimerHandle, this, &UExplosiveComponent::Explode, FuseTime, false);		
	}

}

float UExplosiveComponent::SetFuseTime(float NewFuseTime)
{
	FuseTime = NewFuseTime;
	return NewFuseTime;
}

float UExplosiveComponent::SetDamage(float NewDamage)
{
	Damage = NewDamage;
	return NewDamage;
}

float UExplosiveComponent::SetBlastRadius(float NewBlastRadius)
{
	BlastRadius = NewBlastRadius;
	return NewBlastRadius;
}


