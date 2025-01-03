﻿#pragma once

#include "CoreMinimal.h"
#include "WeaponData.generated.h"

UENUM(BlueprintType)
enum class EWeaponSlot : uint8
{
	Primary = 0 UMETA(DisplayName = "Primary"),
	Secondary = 1 UMETA(DisplayName = "Secondary"),
	Special = 2 UMETA(DisplayName = "Special"),
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Melee = 0 UMETA(DisplayName = "Melee"),
	SemiAutomatic = 1 UMETA(DisplayName = "Semi-Auto"),
	Automatic = 2 UMETA(DisplayName = "Automatic"),
};

UENUM(BlueprintType)
enum class EProjectileType : uint8
{
	Hitscan = 0 UMETA(DisplayName = "Hitscan"),
	Projectile = 1 UMETA(DisplayName = "Projectile"),
};

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	PistolAmmo = 0 UMETA(DisplayName = "PistolAmmo"),
	RifleAmmo = 1 UMETA(DisplayName = "RifleAmmo"),
	ShotgunAmmo = 2 UMETA(DisplayName = "ShotgunAmmo"),
	SniperAmmo = 3 UMETA(DisplayName = "SniperAmmo"),
	ExplosiveAmmo = 4 UMETA(DisplayName = "ExplosiveAmmo"),
	EnergyAmmo = 5 UMETA(DisplayName = "EnergyAmmo"),
};

USTRUCT(BlueprintType)
struct GERA_MOVEMENTSHOOTER_API FWeaponData: public FTableRowBase
{
	GENERATED_BODY()

	bool operator==(const FWeaponData &Other) const
	{
		if (Name == Other.Name) return true;
		return false;
	}

	bool IsNull() const
	{
		if (Name.IsNone())
		{
			return true;
		}
		return false;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Data")
	EWeaponSlot WeaponSlot = EWeaponSlot::Primary;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Data")
	FName Name = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Data")
	EWeaponType WeaponType = EWeaponType::Melee;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Data")
	TSubclassOf<AActor> WeaponActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pickup Settings")
	UStaticMesh* StaticMesh;

	// Color of the outline on the pickup.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pickup Settings")
	FColor PickupOutlineColor = FColor::White;

	// Hitscan will use a Line Trace to immediately hit its target. Projectile is physical object with travel time.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee"), Category="Ammo Settings")
	EProjectileType ProjectileType = EProjectileType::Hitscan;

	// The type of projectile the weapon will shoot.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "ProjectileType == EProjectileType::Projectile || ProjectileType == EProjectileType::ChargedProjectile"), Category="Ammo Settings")
	TSubclassOf<AActor> ProjectileActor;

	// What type of ammo the weapon uses.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee"), Category="Ammo Settings")
	EAmmoType AmmoType = EAmmoType::PistolAmmo;

	// How much ammo it costs per shot fired.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee"), Category="Ammo Settings")
	int AmmoCost = 1;

	// How much ammo the weapon pickup will give the player if the player already owns the weapon.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee"), Category="Ammo Settings")
	int AmmoPickupAmount = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Shooting Settings")
	float Damage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (Tooltip = "In RPM (Rounds Per Minute)",
	EditCondition = "WeaponType == EWeaponType::Automatic || WeaponType == EWeaponType::AutomaticShotgun"), Category="Shooting Settings")
	float FireRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (ClampMin = "1.0", EditCondition = "WeaponType != EWeaponType::Melee"), Category="Aiming Settings")
	float ZoomMultiplier = 1.0f;

	// Allows manually changing the spread when the player is aiming the weapon.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee"), Category="Aiming Settings")
	bool AimingSpreadOverride = false;

	// How much spread the weapon will have when aiming.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee && AimingSpreadOverride"), Category="Aiming Settings")
	float AimingSpreadInDegrees;

	// How much spread the weapon will have when hipfiring.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee"), Category="Aiming Settings")
	float HipfireSpreadInDegrees;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee", ToolTip = "Multiplies the Weapon Spread by this value Each Shot"), Category="Aiming Settings")
	float SpreadMultiplier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee"), Category="Recoil Settings")
	float HipfireRecoilMinX;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee"), Category="Recoil Settings")
	float HipfireRecoilMaxX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee"), Category="Recoil Settings")
	float HipfireRecoilY;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee"), Category="Recoil Settings")
	float AimingRecoilMinX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee"), Category="Recoil Settings")
	float AimingRecoilMaxX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee"), Category="Recoil Settings")
	float AimingRecoilY;
	
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	// meta = (EditCondition = "WeaponType != EWeaponType::Melee"), Category="Recoil Settings")
	// FVector2D HipfireRecoil;
	//
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	// meta = (EditCondition = "WeaponType != EWeaponType::Melee"), Category="Recoil Settings")
	// FVector2D AimingRecoil;

	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	// meta = (Tooltip = "Set to 0 for infinite ammo.",
	// EditCondition = "WeaponType != EWeaponType::Melee"), Category="Ammo Settings")
	// int MagazineCapacity;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != Melee"), Category="Shotgun Settings")
	bool IsShotgun;

	// For Shotguns: How many shots/projectiles will be created when the player shoots.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "isShotgun", ToolTip="For Shotguns: How many shots/projectiles will be created when the player shoots."), Category="Shotgun Settings")
	int PelletCount;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "ProjectileType == EProjectileType::Projectile || ProjectileType == EProjectileType::ChargedProjectile"), Category="Shooting Settings")
	float ProjectileVelocity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Charging")
	bool CanCharge = false;

	// Minimum Time it takes for a charged shot to fire.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (Tooltip = "Minimum Time it takes for a charged shot to fire.",
	EditCondition = "CanCharge", Units="Seconds"), Category="Weapon Charging")
	float MinChargeTime;

	// Time until the weapon reaches its maximum charge.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (Tooltip = "Time until the weapon reaches its maximum charge.",
	EditCondition = "CanCharge", Units="Seconds"), Category="Weapon Charging")
	float MaxChargeTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee"), Category="Explosive")
	bool IsExplosive = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee && IsExplosive"), Category="Explosive")
	float ExplosionRadius = 0.0f;
};