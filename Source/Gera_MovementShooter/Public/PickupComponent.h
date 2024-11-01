// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "WeaponData.h"
#include "Components/ActorComponent.h"
#include "PickupComponent.generated.h"

UENUM(BlueprintType)
enum class EPickupType : uint8
{
	Ammo = 0 UMETA(DisplayName = "Ammo Pickup"),
	Weapon = 1 UMETA(DisplayName = "Weapon Pickup"),
	Equipment = 2 UMETA(DisplayName = "Equipment Pickup"),
	Buff = 3 UMETA(DisplayName = "Buff Pickup"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GERA_MOVEMENTSHOOTER_API UPickupComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPickupComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<AActor> PlayerCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "Meshes")
	UStaticMesh* PistolAmmoMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Meshes")
	UStaticMesh* EnergyAmmoMesh = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Meshes")
	UStaticMesh* RifleAmmoMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Meshes")
	UStaticMesh* ShotgunAmmoMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Meshes")
	UStaticMesh* SniperAmmoMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Meshes")
	UStaticMesh* ExplosiveAmmoMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Meshes")
	UStaticMesh* FragGrenadeMesh = nullptr;
	

	UPROPERTY(EditDefaultsOnly, Category = "Materials")
	UMaterialInstance* OutlineMaterial = nullptr;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicOutline = nullptr;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Type")
	EPickupType PickupType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Type")
	EAmmoType AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Type")
	FDataTableRowHandle WeaponPickup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Type")
	FDataTableRowHandle ItemPickup;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	EPickupType GetPickupType() { return PickupType; }

	EAmmoType GetAmmoType() { return AmmoType; }

	FDataTableRowHandle GetWeaponPickup() { return WeaponPickup; }

	FDataTableRowHandle GetItemPickup() { return ItemPickup; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FWeaponData GetWeaponPickupData() { return *WeaponPickup.GetRow<FWeaponData>(""); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FItemData GetItemPickupData() { return *ItemPickup.GetRow<FItemData>(""); }

	UStaticMesh* GetPistolAmmoMesh() { return PistolAmmoMesh; }

	UStaticMesh* GetEnergyAmmoMesh() { return EnergyAmmoMesh; }

	UStaticMesh* GetRifleAmmoMesh() { return RifleAmmoMesh; }

	UStaticMesh* GetShotgunAmmoMesh() { return ShotgunAmmoMesh; }

	UStaticMesh* GetSniperAmmoMesh() { return SniperAmmoMesh; }

	UStaticMesh* GetExplosiveAmmoMesh() { return ExplosiveAmmoMesh; }

	UFUNCTION(BlueprintCallable)
	void SetOutlineColor(FLinearColor Color);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UMaterialInstanceDynamic* GetOutlineMaterial() { return DynamicOutline; }

	void SetWeaponType(FDataTableRowHandle NewWeapon) { WeaponPickup = NewWeapon; }

	void SetItemPickup(FDataTableRowHandle NewItem) { ItemPickup = NewItem; }
};
