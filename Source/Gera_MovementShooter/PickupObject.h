// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupObject.generated.h"

UENUM(BlueprintType)
enum class PickupType : uint8
{
	PISTOL_WEAPON = 0 UMETA(DisplayName = "Weapon: Pistol"),
	RIFLE_WEAPON = 1 UMETA(DisplayName = "Weapon: Rifle"),
	SHOTGUN_WEAPON = 2 UMETA(DisplayName = "Weapon: Shotgun"),
	GRENADE_LAUNCHER_WEAPON = 3 UMETA(DisplayName = "Weapon: Grenade Launcher"),
	RIFLE_AMMO = 4 UMETA(DisplayName = "Ammo: Rifle"),
	SHOTGUN_AMMO = 5 UMETA(DisplayName = "Ammo: Shotgun"),
	GRENADE_LAUNCHER_AMMO = 6 UMETA(DisplayName = "Ammo: Grenade Launcher"),
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

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* PickupMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Materials")
	UMaterialInterface* RifleAmmoMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Materials")
	UMaterialInterface* ShotgunAmmoMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Materials")
	UMaterialInterface* GrenadeLauncherAmmoMaterial = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Type")
	PickupType CurrentPickupType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo")
	int RifleAmmo = 20;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo")
	int ShotgunAmmo = 4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo")
	int GrenadeLauncherAmmo = 1;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
