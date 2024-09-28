#pragma once

#include "CoreMinimal.h"
#include "WeaponData.generated.h"

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

USTRUCT(BlueprintType)
struct GERA_MOVEMENTSHOOTER_API FWeaponData: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EWeaponType WeaponType = EWeaponType::Melee;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<AActor> WeaponActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType != EWeaponType::Melee", EditConditionHides))
	EProjectileType ProjectileType = EProjectileType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<AActor> ProjectileActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Damage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType == EWeaponType::Automatic", EditConditionHides))
	float FireRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (Tooltip = "Set to 0 for infinite ammo.", EditCondition = "WeaponType != EWeaponType::Melee", EditConditionHides))
	int MagazineCapacity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "WeaponType == EWeaponType::AutomaticShotgun || WeaponType == EWeaponType::SemiAutoShotgun", EditConditionHides))
	int PelletCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "ProjectileType == EProjectileType::Projectile || EProjectileType::ChargedProjectile", EditConditionHides))
	float ProjectileVelocity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "ProjectileType == EProjectileType::ChargedHitscan || EProjectileType::ChargedProjectile", EditConditionHides))
	float MinChargeTime;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
	meta = (EditCondition = "ProjectileType == EProjectileType::ChargedHitscan || EProjectileType::ChargedProjectile", EditConditionHides))
	float MaxChargeTime;
};