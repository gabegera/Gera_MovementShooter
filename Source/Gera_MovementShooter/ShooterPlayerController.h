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
	void ShootHitscan(float SpreadX, float SpreadY, float Damage);

	UFUNCTION(BlueprintCallable)
	void ChargeShot(float TimeToCharge);

	UFUNCTION(BlueprintCallable)
	void FireChargedShot(float Charge);

	UFUNCTION(BlueprintCallable)
	void ResetPistolCharge() { CurrentPistolCharge = 0; }

	UFUNCTION(BlueprintCallable)
	void ShootProjectile(float Velocity);

	UFUNCTION(BlueprintCallable)
	void Shoot();

	// UFUNCTION(BlueprintCallable)
	// void EquipWeapon(EWeaponType NewWeaponType);

	UFUNCTION(BlueprintCallable)
	void AddAmmo(int RifleAddition, int ShotgunAddition, int GrenadeLauncherAddition, PickupType Type);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
