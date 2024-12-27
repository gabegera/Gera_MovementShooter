// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupObject.h"
#include "ConsumablePickup.generated.h"

UENUM(BlueprintType)
enum class EConsumablePickupType : uint8
{
	Ammo = 0 UMETA(DisplayName = "Ammo Pickup"),
	Health = 1 UMETA(DisplayName = "Health Pickup"),
};

UCLASS()
class GERA_MOVEMENTSHOOTER_API AConsumablePickup : public APickupObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Type")
	EConsumablePickupType PickupType;

	
};
