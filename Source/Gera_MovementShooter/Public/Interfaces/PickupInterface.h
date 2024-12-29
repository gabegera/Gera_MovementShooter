// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickupInterface.generated.h"

struct FPickupData;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickupInterface : public UInterface
{
	GENERATED_BODY()
};

class GERA_MOVEMENTSHOOTER_API IPickupInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool PickupAmmo(const EAmmoType AmmoType, const int32 AmmoAmount);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool PickupWeapon(FWeaponData NewWeapon);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool PickupEquipment(FItemData Equipment);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool PickupBuff(FItemData Buff);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool PickupConsumable(const EConsumableEffect ConsumableEffect, const int32 ConsumableAmount);
};
