#pragma once

#include "CoreMinimal.h"
#include "PickupData.generated.h"

UENUM(BlueprintType)
enum class EPickupType : uint8
{
	Weapon = 0 UMETA(DisplayName = "Weapon"),
	Ammo = 1 UMETA(DisplayName = "Ammo"),
	Ability = 2 UMETA(DisplayName = "Ability"),
	Consumable = 3 UMETA(DisplayName = "Consumable"),
	Equipment = 4 UMETA(DisplayName = "Equipment")
};

UENUM(BlueprintType)
enum class EConsumableEffect : uint8
{
	Healing = 0 UMETA(DisplayName = "Healing"),
};

USTRUCT(BlueprintType)
struct GERA_MOVEMENTSHOOTER_API FPickupData: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPickupType PickupType = EPickupType::Weapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName PickupName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* PickupMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor PickupOutlineColor = FColor::Black;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup",
	meta=(EditCondition="PickupType == EPickupType::Weapon", EditConditionHides))
	FDataTableRowHandle WeaponDataTableRowHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup",
	meta=(EditCondition="PickupType == EPickupType::Consumable", EditConditionHides))
	EConsumableEffect ConsumableEffect = EConsumableEffect::Healing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup",
	meta=(EditCondition="PickupType == EPickupType::Ammo", EditConditionHides))
	EAmmoType AmmoType = EAmmoType::None;

	/* How much Ammo to Give the Player or how much Health if consumable, etc... */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup", meta=(EditCondition="PickupType != EPickupType::Ability"))
	int32 PickupAmount;

	
};