// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "PickupObject.h"
#include "DataTables/WeaponData.h"
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

	// ------ COMPONENTS AND POINTERS ------
	
	UPROPERTY(BlueprintReadOnly)
	AShooterPlayerCharacter* PlayerCharacter;

	UPROPERTY(BlueprintReadOnly)
	UHealthComponent* PlayerHealthComp;

	UPROPERTY(BlueprintReadOnly)
	UInventoryComponent* PlayerInventoryComp;

	// ------ INPUT ------

	// Default Mapping Context used for Enhanced Input.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	float MouseSensitivity = 1.0f;

	// ------ DASHING ------

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
	float DashVelocity = 20.0f;

	UPROPERTY()
	FVector DashDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
	UCurveFloat* DashCurve = nullptr;

	UPROPERTY()
	FTimeline DashTimeline;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dash")
	float DashCooldownLength = 2.0f; //This value never changes

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
	float DashCooldownValue = 0.0f; //This value will change

	UFUNCTION(BlueprintCallable)
	void Dash();
	
	// ------ SHOOTING AND WEAPONS ------

	// Arrow Component of Weapon located at the Muzzle.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UArrowComponent* MuzzleArrowComponent;

	// Used for weapons capable of Charging up a more powerful attack.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shooting/Weapons")
	float CurrentWeaponCharge = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shooting/Weapons")
	float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting/Weapons")
	bool InfiniteAmmo = false;

	// Boolean used to control when the player is able to shoot their weapon.
	UFUNCTION(BlueprintCallable)
	bool CanFire();

	// Velocity of Thrown equipment.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
	float ThrowableVelocity = 400.0f;

	// Caches the Default FOV to return to when the player stops aiming.
	UPROPERTY()
	float DefaultFOV;
	
	// ------ RECOIL ------

	// The Target Position that the Recoil will Interpolate Torwards.
	UPROPERTY(BlueprintReadOnly)
	FVector2D RecoilTarget = FVector2D::ZeroVector;

	// The Progress of the Recoils Interpolation Towards RecoilTarget.
	UPROPERTY(BlueprintReadOnly)
	FVector2D RecoilProgress = FVector2D::ZeroVector;

	// How Quickly the Recoil Will Interpolate to the RecoilTarget.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recoil")
	float RecoilInterpSpeed = 60.0f;

	// A Cache of Total Recoil Added, Used In Recoil Recovery.
	UPROPERTY(BlueprintReadOnly)
	FVector2D RecoilCache = FVector2D::ZeroVector;

	// Creates a RecoilTarget that will be interpolated towards.
	UFUNCTION(BlueprintCallable)
	void AddRecoil(FVector2D RecoilAmount);

	// Timer that Updates Recoil Interpolation every frame.
	UPROPERTY()
	FTimerHandle RecoilTimer;

	// Function that gets called by RecoilTimer to Update RecoilProgress.
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

	// Timer used to manage how long the Buff will be active.
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

	// ------ MOVEMENT ------

	UFUNCTION(BlueprintCallable)
	void Move(float InputX, float InputY);

	// Used to Rotate the First Person Camera
	UFUNCTION(BlueprintCallable)
	void AddLookInput(FVector2D Input);

	UFUNCTION(BlueprintCallable)
	void Jump();
	
	UFUNCTION(BlueprintCallable)
	void Crouch();

	UFUNCTION(BlueprintCallable)
	void StopCrouch();

	// ------ SHOOTING / WEAPONS ------

	// Returns the Data of the currently equipped weapon from the WeaponData Data Table.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FWeaponData GetEquippedWeapon();

	// Charges the shot allowing for more damage.
	UFUNCTION(BlueprintCallable)
	void ChargeShot(float MaxCharge);

	UFUNCTION(BlueprintCallable)
	void Shoot();

	// Returns how much spread the weapon will have in degrees.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetShotSpreadInDegrees();

	UFUNCTION(BlueprintCallable)
	void Aim(float ZoomMultiplier);

	UFUNCTION(BlueprintCallable)
	void StopAiming();

	// Returns true if the player is aiming/zooming in their weapon.
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsAiming();

	// Resets weapon variables when trigger of weapon is released.
	UFUNCTION(BlueprintCallable)
	void ResetWeapon();

	UFUNCTION(BlueprintCallable)
	void UseThrowableItem();

	UFUNCTION(BlueprintCallable)
	void UseBuffItem();

public:

	// ------ PLAYER AND COMPONENT GETTERS ------
	
	// Gets ShooterPlayerCharacter.
	UFUNCTION(BlueprintCallable)
	AShooterPlayerCharacter* GetPlayerCharacter() const { return PlayerCharacter; }

	// Gets ShooterPlayerCharacter's Inventory Component.
	UFUNCTION(BlueprintCallable)
	UInventoryComponent* GetInventory() const { return GetPlayerCharacter()->InventoryComponent;}

	// Gets ShooterPlayerCharacter's Health Component.
	UFUNCTION(BlueprintCallable)
	UHealthComponent* GetHealthComponent() const { return PlayerHealthComp; }

	// ------ WEAPON DATA GETTERS ------
	// These are Getters to make Getting Different Weapon Variables faster and cleaner.

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EAmmoType GetAmmoType() { return GetEquippedWeapon().AmmoType; }

	UFUNCTION(Blueprintcallable, BlueprintPure)
	int32 GetEquippedWeaponAmmo() { return GetInventory()->GetAmmo(GetAmmoType()); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EWeaponType GetWeaponType() { return GetEquippedWeapon().WeaponType; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TSubclassOf<AActor> GetProjectileActor() { return GetEquippedWeapon().ProjectileActor;}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EProjectileType GetProjectileType() { return GetEquippedWeapon().ProjectileType; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetProjectileVelocity() { return GetEquippedWeapon().ProjectileVelocity; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetMuzzleLocation() const { return GetPlayerCharacter()->WeaponChildComponent->GetChildActor()->GetComponentByClass<UArrowComponent>()->GetComponentLocation(); }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector2D GetHipfireRecoil();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector2D GetAimingRecoil();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetFireRate() { return GetEquippedWeapon().FireRate; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetDamage() { return GetEquippedWeapon().Damage; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsWeaponShotgun() { return GetEquippedWeapon().IsShotgun; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetPelletCount() { return GetEquippedWeapon().PelletCount; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxChargeTime() { return GetEquippedWeapon().MaxChargeTime; }

	// ------ CAMERA GETTERS ------
	// Getters to make getting the Player's First Person Camera and it's variables faster and cleaner.
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UCameraComponent* GetFPCamera() { return GetPlayerCharacter()->GetFirstPersonCameraComponent(); }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetFPCameraLocation() const { return GetPlayerCharacter()->GetFirstPersonCameraComponent()->GetComponentLocation(); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetFPCameraForward() const { return GetPlayerCharacter()->GetFirstPersonCameraComponent()->GetForwardVector(); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetFPCameraUp() const { return GetPlayerCharacter()->GetFirstPersonCameraComponent()->GetUpVector(); }

	// ------ TICK ------
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
