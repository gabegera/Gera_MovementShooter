// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractablePickup.h"

void AInteractablePickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteractablePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

bool AInteractablePickup::Interact_Implementation(TSubclassOf<AActor> InteractCauser)
{
	//GEngine->AddOnScreenDebugMessage(20, 1, FColor::Cyan, "%s interacted with with Pickup", InteractCauser);

	switch (PickupType) {
	case EInteractablePickupType::Weapon:
		Execute_PickupWeapon(InteractCauser, GetWeaponDataTableRowHandle());
		break;
	case EInteractablePickupType::Equipment:
		//Execute_PickupEquipment(InteractCauser, GetItemPickupData());
		break;
	case EInteractablePickupType::Buff:
		
		break;
	}
	
	return true;
}
