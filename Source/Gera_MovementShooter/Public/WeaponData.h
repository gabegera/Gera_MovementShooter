#pragma once

#include "CoreMinimal.h"
#include "WeaponData.generated.h"

UENUM(BlueprintType)
enum class EWeaponSlot : uint8
{
	Primary = 0 UMETA(DisplayName = "Primary"),
	Secondary = 1 UMETA(DisplayName = "Secondary"),
	Heavy = 2 UMETA(DisplayName = "Heavy"),
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EWeaponSlot WeaponSlot = EWeaponSlot::Primary;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EWeaponType WeaponType = EWeaponType::Melee;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> WeaponActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh* StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee"))
	EProjectileType ProjectileType = EProjectileType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "ProjectileType == EProjectileType::Projectile || ProjectileType == EProjectileType::ChargedProjectile"))
	TSubclassOf<AActor> ProjectileActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee"))
	EAmmoType AmmoType = EAmmoType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee"))
	int AmmoCost = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Damage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (Tooltip = "In RPM (Rounds Per Minute)",
	EditCondition = "WeaponType == EWeaponType::Automatic || WeaponType == EWeaponType::AutomaticShotgun"))
	float FireRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee"))
	float MaxSpread;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (Tooltip = "Set to 0 for infinite ammo.",
	EditCondition = "WeaponType != EWeaponType::Melee"))
	int MagazineCapacity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != Melee"))
	bool IsShotgun;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "isShotgun"))
	int PelletCount;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "ProjectileType == EProjectileType::Projectile || ProjectileType == EProjectileType::ChargedProjectile"))
	float ProjectileVelocity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool CanCharge = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (Tooltip = "In Seconds",
	EditCondition = "CanCharge"))
	float MinChargeTime;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (Tooltip = "In Seconds",
	EditCondition = "CanCharge"))
	float MaxChargeTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee"))
	bool IsExplosive = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee && IsExplosive"))
	float ExplosionRadius = 0.0f;
};