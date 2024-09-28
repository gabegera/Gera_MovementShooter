// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerCharacter.h"
#include "GrenadeProjectile.h"
#include "PickupObject.h"
#include "WeaponData.h"
#include "HealthComponent.h"
#include "Components/ArrowComponent.h"
#include "Engine/DamageEvents.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ShooterPlayerController.generated.h"

UCLASS()
class GERA_MOVEMENTSHOOTER_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	
protected:

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	AShooterPlayerCharacter* PlayerCharacter;

	float CurrentDeltaTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
	float DashVelocity = 10000.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dash")
	float DashTimer = 2.0f; //This value never changes

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
	float DashCooldown = 0.0f; //This value will change


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	float MouseSensitivity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UArrowComponent* MuzzleArrowComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shooting/Weapons")
	float CurrentPistolCharge = 0.0f;
	


	UFUNCTION(BlueprintCallable)
	void Move(float InputX, float InputY);

	UFUNCTION(BlueprintCallable)
	void Look(float InputX, float InputY);

	UFUNCTION(BlueprintCallable)
	void Jump();

	UFUNCTION(BlueprintCallable)
	void Dash(float InputX, float InputY);

	UFUNCTION(BlueprintCallable)
	void Crouch();

	UFUNCTION(BlueprintCallable)
	void StopCrouch();

	UFUNCTION(BlueprintCallable)
	void ShootHitscan(float SpreadX, float SpreadY, float Damage, FVector ShotOrigin);

	UFUNCTION(BlueprintCallable)
	void ChargeShot(float MinCharge, float MaxCharge);

	UFUNCTION(BlueprintCallable)
	void FireChargedShot(float Charge, FVector ShotOrigin);

	UFUNCTION(BlueprintCallable)
	void ResetPistolCharge() { CurrentPistolCharge = 0; }

	UFUNCTION(BlueprintCallable)
	void ShootProjectile(float Velocity, FVector ShotOrigin);

	UFUNCTION(BlueprintCallable)
	void Shoot();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
