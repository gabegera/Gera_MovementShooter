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
	AutomaticShotgun = 3 UMETA(DisplayName = "Automatic Shotgun"),
	SemiAutoShotgun = 4 UMETA(DisplayName = "Semi-Auto Shotgun"),
};

UENUM(BlueprintType)
enum class EProjectileType : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Hitscan = 1 UMETA(DisplayName = "Hitscan"),
	Projectile = 2 UMETA(DisplayName = "Projectile"),
	ChargedHitscan = 3 UMETA(DisplayName = "Charged Hitscan"),
	ChargedProjectile = 4 UMETA(DisplayName = "Charged Projectile"),
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
};

USTRUCT(BlueprintType)
struct GERA_MOVEMENTSHOOTER_API FWeaponData: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EWeaponSlot WeaponSlot = EWeaponSlot::Primary;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EWeaponType WeaponType = EWeaponType::Melee;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> WeaponActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh* StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee", EditConditionHides))
	EProjectileType ProjectileType = EProjectileType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "ProjectileType == EProjectileType::Projectile || ProjectileType == EProjectileType::ChargedProjectile", EditConditionHides))
	TSubclassOf<AActor> ProjectileActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee", EditConditionHides))
	EAmmoType AmmoType = EAmmoType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Damage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (Tooltip = "In RPM (Rounds Per Minute)",
	EditCondition = "WeaponType == EWeaponType::Automatic || WeaponType == EWeaponType::AutomaticShotgun", EditConditionHides))
	float FireRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee", EditConditionHides))
	float MaxSpread;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (Tooltip = "Set to 0 for infinite ammo.",
	EditCondition = "WeaponType != EWeaponType::Melee", EditConditionHides))
	int MagazineCapacity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType == EWeaponType::AutomaticShotgun || WeaponType == EWeaponType::SemiAutoShotgun", EditConditionHides))
	int PelletCount;

	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	// meta = (EditCondition = "WeaponType == EWeaponType::AutomaticShotgun || WeaponType == EWeaponType::SemiAutoShotgun", EditConditionHides))
	// float ShotgunSpread;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "ProjectileType == EProjectileType::Projectile || ProjectileType == EProjectileType::ChargedProjectile", EditConditionHides))
	float ProjectileVelocity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (Tooltip = "In Seconds",
	EditCondition = "ProjectileType == EProjectileType::ChargedHitscan || ProjectileType == EProjectileType::ChargedProjectile", EditConditionHides))
	float MinChargeTime;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (Tooltip = "In Seconds",
	EditCondition = "ProjectileType == EProjectileType::ChargedHitscan || ProjectileType == EProjectileType::ChargedProjectile", EditConditionHides))
	float MaxChargeTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee", EditConditionHides))
	bool IsExplosive = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee && IsExplosive", EditConditionHides))
	float ExplosionRadius = 0.0f;
};