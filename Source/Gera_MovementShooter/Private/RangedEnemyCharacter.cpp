// Fill out your copyright notice in the Description page of Project Settings.

#include "RangedEnemyCharacter.h"
#include "PickupObject.h"

void ARangedEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	
}

void ARangedEnemyCharacter::PickupWeapon(FWeaponData NewWeapon)
{
	InventoryComponent->SwapWeapons(NewWeapon.WeaponSlot, NewWeapon);

	WeaponChildComponent->SetChildActorClass(NewWeapon.WeaponActor->GetClass());
}
