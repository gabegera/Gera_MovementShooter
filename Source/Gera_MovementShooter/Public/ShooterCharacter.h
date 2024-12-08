// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthComponent.h"
#include "InventoryComponent.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

UCLASS()
class GERA_MOVEMENTSHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UChildActorComponent* WeaponChildComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInventoryComponent* InventoryComponent = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	FDataTableRowHandle EquippedWeapon;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FWeaponData GetEquippedWeaponData();

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
    virtual void EquipWeapon(FDataTableRowHandle NewWeapon);
    
    UPROPERTY()
    TSet<AActor*> PickupSet;

public:	
	// Called to bind functionality to input
	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AActor* FindClosestPickup();

	UFUNCTION(BlueprintCallable)
	void PickupItem();

	UFUNCTION(BlueprintCallable)
	void ShootHitscan(float WeaponSpreadInDegrees, FVector ShotOrigin, FVector ShotTarget, float Damage);
	
	UFUNCTION(BlueprintCallable)
	void ShootProjectile(const TSubclassOf<AActor> ProjectileActor, const float WeaponSpreadInDegrees, const FVector ShotOrigin, const FVector ShotTarget, const float ProjectileVelocity, const float Damage);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
