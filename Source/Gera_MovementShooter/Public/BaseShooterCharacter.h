// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthComponent.h"
#include "InventoryComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/PickupInterface.h"
#include "BaseShooterCharacter.generated.h"

class APickupObject;

UCLASS()
class GERA_MOVEMENTSHOOTER_API ABaseShooterCharacter : public ACharacter, public IPickupInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseShooterCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UChildActorComponent* WeaponChildComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInventoryComponent* InventoryComponent = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	FWeaponData EquippedWeapon;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;
	
	UFUNCTION()
    void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
    
    UFUNCTION()
    void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    
    UPROPERTY(EditAnywhere)
    UHealthComponent* HealthComponent = nullptr;
    
    UPROPERTY(EditDefaultsOnly)
    USkeletalMeshComponent* WeaponChildMesh = nullptr;
    
    UFUNCTION(BlueprintCallable)
    virtual void EquipWeapon(FWeaponData NewWeapon);
    
    UPROPERTY()
    TSet<APickupObject*> PickupSet;

public:	
	// Called to bind functionality to input
	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	APickupObject* FindClosestPickup();

	// UFUNCTION(BlueprintCallable)
	// void PickupItem();

	UFUNCTION(BlueprintCallable)
	void ShootHitscan(float WeaponSpreadInDegrees, FVector ShotOrigin, FVector ShotTarget, float Damage);
	
	UFUNCTION(BlueprintCallable)
	void ShootProjectile(const TSubclassOf<AActor> ProjectileActor, const float WeaponSpreadInDegrees, const FVector ShotOrigin, const FVector ShotTarget, const float ProjectileVelocity, const float Damage);

	UFUNCTION(BlueprintCallable)
	bool PickupAmmo_Implementation(const EAmmoType AmmoType, const int32 AmmoAmount) override;

	UFUNCTION(BlueprintCallable)
	bool PickupWeapon_Implementation(FWeaponData WeaponDataTableRowHandle) override;

	UFUNCTION(BlueprintCallable)
	bool PickupConsumable_Implementation(const EConsumableEffect ConsumableEffect, const int32 ConsumableAmount) override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
