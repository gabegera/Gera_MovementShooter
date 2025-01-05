#pragma once

#include "CoreMinimal.h"
#include "EquipmentData.generated.h"

class AThrowableActor;

USTRUCT(BlueprintType)
struct GERA_MOVEMENTSHOOTER_API FEquipmentData: public FTableRowBase
{
	GENERATED_BODY()

	bool operator==(const FWeaponData &Other) const
	{
		if (EquipmentName == Other.Name) return true;
		return false;
	}

	bool IsNull() const
	{
		if (EquipmentName.IsNone())
		{
			return true;
		}
		return false;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName EquipmentName = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* EquipmentMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<AThrowableActor> EquipmentActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FColor PickupOutlineColor = FColor::Black;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Explosive")
	bool IsExplosive = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Explosive", meta=(EditCondition="IsExplosive"))
	float BlastRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Explosive", meta=(EditCondition="IsExplosive"))
	float FuseTime;
};