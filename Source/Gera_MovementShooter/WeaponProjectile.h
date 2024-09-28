// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponData.h"
#include "HealthComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "WeaponProjectile.generated.h"

UCLASS()
class GERA_MOVEMENTSHOOTER_API AWeaponProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	USphereComponent* SphereCollision;

	UFUNCTION(BlueprintCallable)
	void DealDamage(AActor* Collider, AActor* PlayerCharacter, AController* PlayerController);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
