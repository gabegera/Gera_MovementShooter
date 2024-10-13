// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "ThrowableActor.generated.h"

UCLASS()
class GERA_MOVEMENTSHOOTER_API AThrowableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThrowableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle FuseTimerHandle;

	UPROPERTY(EditAnywhere)
	UParticleSystem* GrenadeExplosionParticle = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	float BlastRadius;

	UPROPERTY(EditDefaultsOnly)
	float Damage;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* SphereComponent;

	

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UStaticMeshComponent* GetStaticMeshComponent() { return StaticMesh; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	USphereComponent* GetSphereComponent() { return SphereComponent; }

	UFUNCTION(BlueprintCallable)
	void SetBlastRadius(float NewRadius) { BlastRadius = NewRadius; }

	UFUNCTION(BlueprintCallable)
	void SetDamage(float NewDamage) { Damage = NewDamage; }

	UFUNCTION(BlueprintCallable)
	void StartFuseTimer(float FuseTime);

	UFUNCTION(BlueprintCallable)
	void Explode();
	
};
