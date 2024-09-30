// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponData.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "PickupObject.generated.h"

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
	
	UFUNCTION()
	void BeginWeaponSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void EndWeaponSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USphereComponent* WeaponSphereTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USphereComponent* PickupSphereTrigger;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* PickupMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meshes")
	UStaticMesh* PistolAmmoMesh = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meshes")
	UStaticMesh* RifleAmmoMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meshes")
	UStaticMesh* ShotgunAmmoMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meshes")
	UStaticMesh* SniperAmmoMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Meshes")
	UStaticMesh* ExplosiveAmmoMesh = nullptr;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Materials")
	UMaterialInstance* PrimaryOutlineMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Materials")
	UMaterialInstance* SecondaryOutlineMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Materials")
	UMaterialInstance* HeavyOutlineMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Materials")
	UMaterialInstance* EquipmentOutlineMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Materials")
	UMaterialInstance* BuffOutlineMaterial = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Type")
	EPickupType PickupType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Type")
	EAmmoType AmmoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Type")
	FDataTableRowHandle WeaponPickup;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
