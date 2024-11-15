#pragma once

#include "CoreMinimal.h"
#include "WeaponData.generated.h"

UENUM(BlueprintType)
enum class EWeaponSlot : uint8
{
	Primary = 0 UMETA(DisplayName = "Primary"),
	Secondary = 1 UMETA(DisplayName = "Secondary"),
	Heavy = 2 UMETA(DisplayName = "Heavy"),
	AnySlot = 3 UMETA(DisplayName = "AnySlot"),
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
	None = 0 UMETA(DisplayName = "None"),
	Hitscan = 1 UMETA(DisplayName = "Hitscan"),
	Projectile = 2 UMETA(DisplayName = "Projectile"),
};

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	PistolAmmo = 1 UMETA(DisplayName = "PistolAmmo"),
	RifleAmmo = 2 UMETA(DisplayName = "RifleAmmo"),
	ShotgunAmmo = 3 UMETA(DisplayName = "ShotgunAmmo"),
	SniperAmmo = 4 UMETA(DisplayName = "SniperAmmo"),
	ExplosiveAmmo = 5 UMETA(DisplayName = "ExplosiveAmmo"),
	EnergyAmmo = 6 UMETA(DisplayName = "EnergyAmmo"),
};

USTRUCT(BlueprintType)
struct GERA_MOVEMENTSHOOTER_API FWeaponData: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Data")
	EWeaponSlot WeaponSlot = EWeaponSlot::Primary;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Data")
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Data")
	EWeaponType WeaponType = EWeaponType::Melee;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Data")
	TSubclassOf<AActor> WeaponActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pickup Settings")
	UStaticMesh* StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pickup Settings")
	FColor OutlineColor = FColor::White;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee"), Category="Ammo Settings")
	EProjectileType ProjectileType = EProjectileType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "ProjectileType == EProjectileType::Projectile || ProjectileType == EProjectileType::ChargedProjectile"), Category="Ammo Settings")
	TSubclassOf<AActor> ProjectileActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee"), Category="Ammo Settings")
	EAmmoType AmmoType = EAmmoType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee"), Category="Shooting Settings")
	int AmmoCost = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Shooting Settings")
	float Damage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (Tooltip = "In RPM (Rounds Per Minute)",
	EditCondition = "WeaponType == EWeaponType::Automatic || WeaponType == EWeaponType::AutomaticShotgun"), Category="Shooting Settings")
	float FireRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (ClampMin = "1.0", EditCondition = "WeaponType != EWeaponType::Melee"), Category="Aiming Settings")
	float ZoomMultiplier = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee"), Category="Aiming Settings")
	bool AimingSpreadOverride = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee && AimingSpreadOverride"), Category="Aiming Settings")
	float AimingSpreadInDegrees;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee"), Category="Aiming Settings")
	float SpreadInDegrees;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee", ToolTip = "Multiplies the Weapon Spread by this value Each Shot"), Category="Aiming Settings")
	float SpreadMultiplier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee"), Category="Recoil Settings")
	FVector2D HipfireRecoil;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee"), Category="Recoil Settings")
	FVector2D AimingRecoil;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (Tooltip = "Set to 0 for infinite ammo.",
	EditCondition = "WeaponType != EWeaponType::Melee"), Category="Ammo Settings")
	int MagazineCapacity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != Melee"), Category="Shotgun Settings")
	bool IsShotgun;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "isShotgun"), Category="Shotgun Settings")
	int PelletCount;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "ProjectileType == EProjectileType::Projectile || ProjectileType == EProjectileType::ChargedProjectile"), Category="Shooting Settings")
	float ProjectileVelocity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Charging")
	bool CanCharge = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (Tooltip = "In Seconds",
	EditCondition = "CanCharge"), Category="Weapon Charging")
	float MinChargeTime;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (Tooltip = "In Seconds",
	EditCondition = "CanCharge"), Category="Weapon Charging")
	float MaxChargeTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee"), Category="Explosive")
	bool IsExplosive = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee && IsExplosive"), Category="Explosive")
	float ExplosionRadius = 0.0f;
};