// Fill out your copyright notice in the Description page of Project Settings.

#include "RangedEnemyCharacter.h"
#include "BasePickupObject.h"

void ARangedEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	
}

void ARangedEnemyCharacter::PickupWeapon(FWeaponData NewWeapon)
{
	InventoryComponent->AddWeapon(NewWeapon);

	WeaponChildComponent->SetChildActorClass(NewWeapon.WeaponActor->GetClass());
}
