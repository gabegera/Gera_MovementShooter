// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterPlayerCharacter.h"
#include "EnemyCharacter.generated.h"

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	Melee = 0 UMETA(DisplayName = "Melee"),
	Ranged = 1 UMETA(DisplayName = "Ranged"),
};

UCLASS()
class GERA_MOVEMENTSHOOTER_API AEnemyCharacter : public AShooterCharacter 
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EEnemyType GetEnemyType() { return EnemyType; }

	UFUNCTION(BLueprintCallable)
	EEnemyType SetEnemyType(EEnemyType NewEnemyType);

	// UFUNCTION(BlueprintCallable, BlueprintPure)
	// UChildActorComponent* GetWeaponChildComponent() { return WeaponChildComponent; }
	//
	// UFUNCTION(BlueprintCallable, BlueprintPure)
	// class UHealthComponent* GetHealthComponent() { return HealthComponent; }
	//
	// UFUNCTION(BlueprintCallable, BlueprintPure)
	// class UInventoryComponent* GetInventoryComponent() { return InventoryComponent; }

	

	UFUNCTION(BlueprintCallable)
	void PickupWeapon(APickupObject* Pickup);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// UPROPERTY(EditAnywhere)
	// UChildActorComponent* WeaponChildComponent;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadOnly)
	// class UHealthComponent* HealthComponent;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadOnly)
	// class UInventoryComponent* InventoryComponent;

    UPROPERTY(EditAnywhere)
	EEnemyType EnemyType = EEnemyType::Melee;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
};
