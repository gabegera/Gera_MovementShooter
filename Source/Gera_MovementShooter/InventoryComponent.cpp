// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	AmmoMap.Add(EAmmoType::PistolAmmo, 20);
	AmmoMap.Add(EAmmoType::RifleAmmo, 60);
	AmmoMap.Add(EAmmoType::ShotgunAmmo, 16);
	AmmoMap.Add(EAmmoType::SniperAmmo, 4);
	AmmoMap.Add(EAmmoType::ExplosiveAmmo, 4);
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UInventoryComponent::SwapWeapons(EWeaponSlot WeaponSlot, FDataTableRowHandle NewWeapon)
{
	switch (WeaponSlot)
	{
	case EWeaponSlot::Primary:
		PrimaryWeapon = NewWeapon;
		break;
	case EWeaponSlot::Secondary:
		SecondaryWeapon = NewWeapon;
		break;
	case EWeaponSlot::Heavy:
		HeavyWeapon = NewWeapon;
		break;
	}
}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::AddAmmo(EAmmoType AmmoType, int AmmoAmount)
{
	AmmoMap.Add(AmmoType, AmmoAmount);
}

