// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerCharacter.h"
#include "GrenadeProjectile.h"
#include "PickupObject.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ShooterPlayerController.generated.h"

UENUM(BlueprintType)
enum class WeaponType : uint8
{
	PISTOL = 0 UMETA(DisplayName = "Pistol"),
	RIFLE = 1 UMETA(DisplayName = "Rifle"),
	SHOTGUN = 2 UMETA(DisplayName = "Shotgun"),
	GRENADELAUNCHER = 3 UMETA(DisplayName = "Grenade Launcher"),
};

UCLASS()
class GERA_MOVEMENTSHOOTER_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	
protected:

	virtual void BeginPlay() override;


	AShooterPlayerCharacter* PlayerCharacter;

	float CurrentDeltaTime;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jump")
	int MaxJumps = 2;


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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting/Weapons")
	WeaponType EquippedWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shooting/Weapons")
	float CurrentPistolCharge = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shooting/Weapons")
	float PistolChargeDamageMultiplier = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shooting/Weapons")
	float PistolChargeSpeed = 2.0f; // Seconds to Charge

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shooting/Weapons")
	int RifleAmmo = 20;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shooting/Weapons")
	float RifleSpread = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shooting/Weapons")
	int ShotgunAmmo = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shooting/Weapons")
	int ShotgunPellets = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shooting/Weapons")
	float ShotgunSpread = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shooting/Weapons")
	int GrenadeLauncherAmmo = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shooting/Weapons")
	TSubclassOf<AGrenadeProjectile> GrenadeProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shooting/Weapons")
	float GrenadeVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shooting/Weapons")
	float RifleFireRate = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shooting/Weapons")
	float FireRate = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shooting/Weapons")
	float WeaponDamage = 5.0f;


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
	void ShootHitscan(float SpreadX, float SpreadY);

	UFUNCTION(BlueprintCallable)
	void ChargeShot(float TimeToCharge);

	UFUNCTION(BlueprintCallable)
	void FireChargedShot(float Charge);

	UFUNCTION(BlueprintCallable)
	void ShootProjectile(float Velocity);

	UFUNCTION(BlueprintCallable)
	void Shoot();

	UFUNCTION(BlueprintCallable)
	void EquipPistol();

	UFUNCTION(BlueprintCallable)
	void EquipRifle();

	UFUNCTION(BlueprintCallable)
	void EquipShotgun();

	UFUNCTION(BlueprintCallable)
	void EquipGrenadeLauncher();

	UFUNCTION(BlueprintCallable)
	void ResetFireRate() { FireRate = 0; }

	UFUNCTION(BlueprintCallable)
	void AddAmmo(int RifleAddition, int ShotgunAddition, int GrenadeLauncherAddition, PickupType Type);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
