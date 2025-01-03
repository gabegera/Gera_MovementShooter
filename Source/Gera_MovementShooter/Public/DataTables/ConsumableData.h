#pragma once

#include "CoreMinimal.h"
#include "ConsumableData.generated.h"

UENUM(BlueprintType)
enum class EConsumableEffect : uint8
{
	Health = 0 UMETA(DisplayName = "Health"),
	Armor = 1 UMETA(DisplayName = "Armor"),
	DamageBoost = 2 UMETA(DisplayName = "Damage Boost"),
	SpeedBoost = 3 UMETA(DisplayName = "Speed Boost"),
};

USTRUCT(BlueprintType)
struct GERA_MOVEMENTSHOOTER_API FConsumableData: public FTableRowBase
{
	GENERATED_BODY()

	bool operator==(const FWeaponData &Other) const
	{
		if (ConsumableName == Other.Name) return true;
		return false;
	}

	bool IsNull() const
	{
		if (ConsumableName.IsNone())
		{
			return true;
		}
		return false;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ConsumableName = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* ConsumableMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FColor PickupOutlineColor = FColor::Black;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Consumable")
	EConsumableEffect ConsumableEffect = EConsumableEffect::Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Consumable")
	float PickupAmount = 20.0f;
};