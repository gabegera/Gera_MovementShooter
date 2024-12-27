// Fill out your copyright notice in the Description page of Project Settings.

#include "RangedEnemyCharacter.h"
#include "PickupObject.h"

void ARangedEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	
}

void ARangedEnemyCharacter::PickupWeapon(APickupObject* Pickup)
{
	InventoryComponent->SwapWeapons(EWeaponSlot::Primary, Pickup->GetWeaponDataTableRowHandle());

	WeaponChildComponent->SetChildActorClass(Pickup->GetWeaponDataTableRowHandle().GetRow<FWeaponData>("")->WeaponActor->GetClass());
}
