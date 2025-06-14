// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "PickupObject.h"
#include "WeaponData.h"
#include "ItemData.h"
#include "HealthComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/ArrowComponent.h"
#include "Engine/DamageEvents.h"
#include "ThrowableActor.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ShooterPlayerController.generated.h"

UCLASS()
class GERA_MOVEMENTSHOOTER_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()


protected:

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	AShooterPlayerCharacter* PlayerCharacter;

	UPROPERTY(BlueprintReadOnly)
	UHealthComponent* PlayerHealthComp;

	UPROPERTY(BlueprintReadOnly)
	UInventoryComponent* PlayerInventoryComp;

	float CurrentDeltaTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	float MouseSensitivity = 1.0f;

	// ------ DASHING ------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
	float DashVelocity = 20.0f;

	FVector DashDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
	UCurveFloat* DashCurve = nullptr;
	
	FTimeline DashTimeline;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dash")
	float DashCooldownLength = 2.0f; //This value never changes

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
	float DashCooldownValue = 0.0f; //This value will change

	UFUNCTION(BlueprintCallable)
	void Dash();

	
	
	// ------ SHOOTING AND WEAPONS ------
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UArrowComponent* MuzzleArrowComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shooting/Weapons")
	float CurrentWeaponCharge = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shooting/Weapons")
	float FireRate;

	UPROPERTY(BlueprintReadOnly, Category = "Shooting/Weapons")
	bool SemiAutoCanFire;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	float ThrowableVelocity = 400.0f;

	UPROPERTY()
	float DefaultFOV;

	UPROPERTY(EditAnywhere, Category = "Shooting/Weapons")
	FTimeline AimingTimeline;
	
	// ------ RECOIL ------

	UPROPERTY(BlueprintReadOnly)
	FVector2D RecoilTarget = FVector2D::ZeroVector;
	
	UPROPERTY(BlueprintReadOnly)
	FVector2D RecoilProgress = FVector2D::ZeroVector;

	// How Quickly the Recoil Will Interpolate to the RecoilTarget.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recoil")
	float RecoilInterpSpeed = 60.0f;

	// A Cache of Total Recoil Added, Used In Recoil Recovery.
	UPROPERTY(BlueprintReadOnly)
	FVector2D RecoilCache = FVector2D::ZeroVector;

	UFUNCTION(BlueprintCallable)
	void AddRecoil(FVector2D RecoilAmount);

	FTimerHandle RecoilTimer;

	// UPROPERTY(BlueprintReadWrite)
	// FVector2D MouseRecoilReturnLimit = FVector2D(5, 5);
	//
	// UPROPERTY(BlueprintReadWrite)
	// FVector2D MouseRecoilTracker = FVector2D::ZeroVector;

	UFUNCTION()
	void UpdateRecoil();

	// ------ BUFFS ------
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Buffs")
	float SpeedBoostMultiplier = 3.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Buffs")
	bool IsDamageBoostActive = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Buffs")
	float DamageBoostMultiplier = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Buffs")
	float SlowTimeDivision = 3.0f;

	UPROPERTY()
	FTimerHandle BuffTimer;

	UFUNCTION(BlueprintCallable)
	void UseSpeedBoost();

	UFUNCTION()
	void StopSpeedBoost();

	UFUNCTION(BlueprintCallable)
	void UseDamageBoost();

	UFUNCTION()
	void StopDamageBoost();

	UFUNCTION(BlueprintCallable)
	void UseSlowTime();

	UFUNCTION()
	void StopSlowTime() const;
	

	UFUNCTION(BlueprintCallable)
	void Move(float InputX, float InputY);

	UFUNCTION(BlueprintCallable)
	void AddLookInput(FVector2D Input);

	UFUNCTION(BlueprintCallable)
	void Jump();
	
	UFUNCTION(BlueprintCallable)
	void Crouch();

	UFUNCTION(BlueprintCallable)
	void StopCrouch();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FWeaponData GetEquippedWeaponData();

	UFUNCTION(BlueprintCallable)
	void ChargeShot(float MaxCharge);

	UFUNCTION(BlueprintCallable)
	void Shoot(bool InfiniteAmmo = false);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetShotSpreadInDegrees();

	UFUNCTION(BlueprintCallable)
	void Aim(float ZoomMultiplier);

	UFUNCTION(BlueprintCallable)
	void StopAiming();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsAiming();

	UFUNCTION(BlueprintCallable)
	void ResetWeapon();

	UFUNCTION(BlueprintCallable)
	void UseThrowableItem();

	UFUNCTION(BlueprintCallable)
	void UseBuffItem();

public:

	AShooterPlayerCharacter* GetPlayerCharacter() const { return PlayerCharacter; }

	UInventoryComponent* GetInventory() const { return GetPlayerCharacter()->InventoryComponent;}

	// ------ WEAPON DATA GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EAmmoType GetAmmoType() { return GetEquippedWeaponData().AmmoType; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EWeaponType GetWeaponType() { return GetEquippedWeaponData().WeaponType; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TSubclassOf<AActor> GetProjectileActor() { return GetEquippedWeaponData().ProjectileActor;}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EProjectileType GetProjectileType() { return GetEquippedWeaponData().ProjectileType; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetProjectileVelocity() { return GetEquippedWeaponData().ProjectileVelocity; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetMuzzleLocation() { return GetPlayerCharacter()->WeaponChildComponent->GetChildActor()->GetComponentByClass<UArrowComponent>()->GetComponentLocation(); }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector2D GetHipfireRecoil();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector2D GetAimingRecoil();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetFireRate() { return GetEquippedWeaponData().FireRate; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetDamage() { return GetEquippedWeaponData().Damage; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsWeaponShotgun() { return GetEquippedWeaponData().IsShotgun; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetPelletCount() { return GetEquippedWeaponData().PelletCount; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxChargeTime() { return GetEquippedWeaponData().MaxChargeTime; }

	// ------ CAMERA GETTERS ------
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UCameraComponent* GetFPCamera() { return GetPlayerCharacter()->GetFirstPersonCameraComponent(); }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetFPCameraLocation() { return GetPlayerCharacter()->GetFirstPersonCameraComponent()->GetComponentLocation(); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetFPCameraForward() { return GetPlayerCharacter()->GetFirstPersonCameraComponent()->GetForwardVector(); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetFPCameraUp() { return GetPlayerCharacter()->GetFirstPersonCameraComponent()->GetUpVector(); }
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
