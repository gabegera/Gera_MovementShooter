// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ExplosiveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GERA_MOVEMENTSHOOTER_API UExplosiveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UExplosiveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ExplosionEffect = nullptr;

	UPROPERTY(EditDefaultsOnly)
	float FuseTime;

	FTimerHandle FuseTimerHandle;
	
	UPROPERTY(EditDefaultsOnly)
	float BlastRadius;

	UPROPERTY(EditDefaultsOnly)
	float Damage;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Explode();

	UFUNCTION(BlueprintCallable)
	void StartFuseTimer();

	UFUNCTION(BlueprintCallable)
	float SetFuseTime(float NewFuseTime);

	UFUNCTION(BlueprintCallable)
	float SetDamage(float NewDamage);

	UFUNCTION(BlueprintCallable)
	float SetBlastRadius(float NewBlastRadius);

	
};
