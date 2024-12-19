// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "WeaponData.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "PickupObject.generated.h"

class AShooterPlayerCharacter;

UENUM(BlueprintType)
enum class EPickupType : uint8
{
	Ammo = 0 UMETA(DisplayName = "Ammo Pickup"),
	Weapon = 1 UMETA(DisplayName = "Weapon Pickup"),
	Equipment = 2 UMETA(DisplayName = "Equipment Pickup"),
	Buff = 3 UMETA(DisplayName = "Buff Pickup"),
};

UCLASS()
class GERA_MOVEMENTSHOOTER_API APickupObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void BeginPickupSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY()
	AShooterPlayerCharacter* PlayerCharacter;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Type",
	meta = (EditCondition = "PickupType == EPickupType::Ammo", EditConditionHides))
	EAmmoType AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Type",
	meta = (EditCondition = "PickupType == EPickupType::Weapon", EditConditionHides))
	FDataTableRowHandle WeaponPickup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Type",
	meta = (EditCondition = "PickupType == EPickupType::Equipment || PickupType == EPickupType::Buff", EditConditionHides))
	FDataTableRowHandle ItemPickup;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USphereComponent* InteractSphereTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USphereComponent* PickupSphereTrigger;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* PickupMesh = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EPickupType GetPickupType() { return PickupType; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EAmmoType GetAmmoType() { return AmmoType; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FDataTableRowHandle GetWeaponPickup() { return WeaponPickup; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FDataTableRowHandle GetItemPickup() { return ItemPickup; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FWeaponData GetWeaponPickupData() { return *WeaponPickup.GetRow<FWeaponData>(""); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FItemData GetItemPickupData() { return *ItemPickup.GetRow<FItemData>(""); }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UStaticMesh* GetPistolAmmoMesh() { return PistolAmmoMesh; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UStaticMesh* GetEnergyAmmoMesh() { return EnergyAmmoMesh; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UStaticMesh* GetRifleAmmoMesh() { return RifleAmmoMesh; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UStaticMesh* GetShotgunAmmoMesh() { return ShotgunAmmoMesh; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UStaticMesh* GetSniperAmmoMesh() { return SniperAmmoMesh; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UStaticMesh* GetExplosiveAmmoMesh() { return ExplosiveAmmoMesh; }

	UFUNCTION(BlueprintCallable)
	void SetOutlineColor(FLinearColor Color);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UMaterialInstanceDynamic* GetOutlineMaterial() { return DynamicOutline; }

	UFUNCTION(BlueprintCallable)
	void RefreshPickup();

	UFUNCTION(BlueprintCallable)
	FDataTableRowHandle SetWeaponPickup(FDataTableRowHandle NewWeapon);

	UFUNCTION(BlueprintCallable)
	FDataTableRowHandle SetItemPickup(FDataTableRowHandle NewItem);

};
