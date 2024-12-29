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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Consumable")
	EConsumableEffect ConsumableEffect = EConsumableEffect::Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Consumable")
	float ConsumableAmount = 20.0f;
};