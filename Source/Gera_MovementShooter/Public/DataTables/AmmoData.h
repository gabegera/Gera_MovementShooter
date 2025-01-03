#pragma once

#include "CoreMinimal.h"
#include "WeaponData.h"
#include "AmmoData.generated.h"

USTRUCT(BlueprintType)
struct GERA_MOVEMENTSHOOTER_API FAmmoData: public FTableRowBase
{
	GENERATED_BODY()

	bool operator==(const FWeaponData &Other) const
	{
		if (AmmoName == Other.Name) return true;
		return false;
	}

	bool IsNull() const
	{
		if (AmmoName.IsNone())
		{
			return true;
		}
		return false;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName AmmoName = NAME_None;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* AmmoMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FColor PickupOutlineColor = FColor::Black;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAmmoType AmmoType = EAmmoType::EnergyAmmo;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 PickupAmount = 20;
};