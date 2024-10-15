#pragma once

#include "CoreMinimal.h"
#include "ItemData.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Throwable = 0 UMETA(DisplayName = "Throwable"),
	Buff = 1 UMETA(DisplayName = "Buff"),
};

UENUM(BlueprintType)
enum class EBuffEffects : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Heal = 1 UMETA(DisplayName = "Heal"),
	SpeedBoost = 2 UMETA(DisplayName = "Speed Boost"),
	DamageBoost = 3 UMETA(DisplayName = "Damage Boost"),
	SlowTime = 4 UMETA(DisplayName = "Slow Time"),
};

USTRUCT(BlueprintType)
struct GERA_MOVEMENTSHOOTER_API FItemData: public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EItemType ItemType = EItemType::Throwable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition = "ItemType == EItemType::Buff", EditConditionHides))
	EBuffEffects BuffEffect = EBuffEffects::None;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> ItemActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh* ItemMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name = NAME_None;

	//Throwable Variables
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition = "ItemType == EItemType::Throwable", EditConditionHides))
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition = "ItemType == EItemType::Throwable", EditConditionHides))
	float FuseTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition = "ItemType == EItemType::Throwable", EditConditionHides))
	float BlastRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition = "ItemType == EItemType::Throwable && BlastRadius != 0", EditConditionHides))
	float DamageFalloff;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition = "ItemType == EItemType::Throwable", EditConditionHides))
	bool IsSticky;


	// Buff Variables

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition = "ItemType == EItemType::Buff && BuffEffect == EBuffEffects::Heal", EditConditionHides))
	float HealAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition = "ItemType == EItemType::Buff && BuffEffect != EBuffEffects::Heal", EditConditionHides))
	float BuffMultiplier;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition = "ItemType == EItemType::Buff && BuffEffect != EBuffEffects::Heal", EditConditionHides))
	float BuffDuration;
};
