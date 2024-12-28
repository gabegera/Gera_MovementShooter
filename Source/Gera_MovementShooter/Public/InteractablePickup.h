// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupObject.h"
#include "Interfaces/InteractInterface.h"
#include "InteractablePickup.generated.h"

UENUM(BlueprintType)
enum class EInteractablePickupType : uint8
{
	Weapon = 0 UMETA(DisplayName = "Weapon Pickup"),
	Equipment = 1 UMETA(DisplayName = "Equipment Pickup"),
	Buff = 3 UMETA(DisplayName = "Buff Pickup"),
};

UCLASS()
class GERA_MOVEMENTSHOOTER_API AInteractablePickup : public APickupObject, public IInteractInterface
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Type")
	EInteractablePickupType PickupType;


public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	bool Interact_Implementation(AActor* InteractCauser) override;
	
	
};
