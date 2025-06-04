// Fill out your copyright notice in the Description page of Project Settings.

#include "RangedEnemyCharacter.h"
#include "PickupObject.h"

void ARangedEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	
}

void ARangedEnemyCharacter::PickupWeapon(APickupObject* Pickup)
{
	InventoryComponent->SwapWeapons(EWeaponSlot::Primary, Pickup->GetComponentByClass<UPickupComponent>()->GetWeaponPickup());

	WeaponChildComponent->SetChildActorClass(Pickup->GetComponentByClass<UPickupComponent>()->GetWeaponPickupData().WeaponActor->GetClass());
}
