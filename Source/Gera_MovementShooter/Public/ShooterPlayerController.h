// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerCharacter.h"
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

protected:

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	AShooterPlayerCharacter* PlayerCharacter;

	UPROPERTY(BlueprintReadOnly)
	UHealthComponent* PlayerHealthComp;

	UPROPERTY(BlueprintReadOnly)
	UInventoryComponent* PlayerInventoryComp;

	float CurrentDeltaTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
	float DashVelocity = 10000.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dash")
	float DashTimer = 2.0f; //This value never changes

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash")
	float DashCooldown = 0.0f; //This value will change

	UPROPERTY()
	float DefaultAirFriction = 0.0f;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	float MouseSensitivity = 1.0f;

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

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FWeaponData GetEquippedWeaponData() { return *PlayerCharacter->EquippedWeapon.GetRow<FWeaponData>(""); }

	UFUNCTION(BlueprintCallable)
	void ShootHitscan(float SpreadX, float SpreadY, FVector ShotOrigin, float Damage);

	UFUNCTION(BlueprintCallable)
	void ChargeShot(float MaxCharge);

	UFUNCTION(BlueprintCallable)
	void ShootProjectile(float SpreadX, float SpreadY, float Velocity, FVector ShotOrigin);

	UFUNCTION(BlueprintCallable)
	void Shoot();

	UFUNCTION(BlueprintCallable)
	void ResetWeapon();

	UFUNCTION(BlueprintCallable)
	void UseThrowableItem();

	UFUNCTION(BlueprintCallable)
	void UseBuffItem();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
