// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseEnemyCharacter.h"
#include "Components/ArrowComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "BaseEnemyAIController.generated.h"



UCLASS()
class GERA_MOVEMENTSHOOTER_API ABaseEnemyAIController : public AAIController
{
	GENERATED_BODY()

protected:

	ABaseEnemyAIController();

	UPROPERTY(VisibleDefaultsOnly, Category = "AI")
	class UAISenseConfig_Sight* SightConfig;
	
	UPROPERTY()
	ABaseEnemyCharacter* EnemyCharacter;
	
	// UPROPERTY(EditAnywhere, BlueprintReadOnly)
	// UAIPerceptionComponent* AIPerceptionComponent = nullptr;

	UPROPERTY()
	FTimerHandle FireRateTimerHandle;

	UPROPERTY()
	bool CanShoot = true;

public:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ABaseEnemyCharacter* GetEnemyCharacter();

	UFUNCTION(BlueprintCallable)
	void Shoot(FVector TargetLocation);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetShotSpreadInDegrees();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetCanShoot() { return CanShoot; }

	UFUNCTION(BlueprintCallable)
	void CanShootEnable() { CanShoot = true; }

	UFUNCTION(BlueprintCallable)
	void CanShootDisable() { CanShoot = false;}

	// ------ WEAPON DATA GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FWeaponData GetEquippedWeapon();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EAmmoType GetAmmoType() { return GetEquippedWeapon().AmmoType; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EWeaponType GetWeaponType() { return GetEquippedWeapon().WeaponType; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TSubclassOf<AActor> GetProjectileActor() { return GetEquippedWeapon().ProjectileActor;}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EProjectileType GetProjectileType() { return GetEquippedWeapon().ProjectileType; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetProjectileVelocity() { return GetEquippedWeapon().ProjectileVelocity; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetMuzzleLocation() { return GetEnemyCharacter()->WeaponChildComponent->GetChildActor()->GetComponentByClass<UArrowComponent>()->GetComponentLocation(); }

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

	virtual void Tick(float DeltaTime) override;
};
