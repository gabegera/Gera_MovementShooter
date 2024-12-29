// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComponent::PostInitProperties()
{
	Super::PostInitProperties();
}


void UInventoryComponent::SwapWeapons(EWeaponSlot WeaponSlot, FWeaponData NewWeapon)
{
	switch (WeaponSlot)
	{
	case EWeaponSlot::Primary:
		PrimaryWeapon = NewWeapon;
		break;
	case EWeaponSlot::Secondary:
		SecondaryWeapon = NewWeapon;
		break;
	case EWeaponSlot::Special:
		SpecialWeapon = NewWeapon;
		break;
	}
}

void UInventoryComponent::SwapEquipment(FDataTableRowHandle NewEquipment)
{
	EquipmentSlot = NewEquipment;
}

void UInventoryComponent::SwapSupportItem(FDataTableRowHandle NewBuffItem)
{
	SupportItemSlot = NewBuffItem;
}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int UInventoryComponent::GetAmmo(EAmmoType AmmoType)
{
	return AmmoMap.FindRef(AmmoType);
}

int UInventoryComponent::GetEquipment(FName EquipmentName)
{
	return EquipmentMap.FindRef(EquipmentName);
}


void UInventoryComponent::AddAmmo(EAmmoType AmmoType, int AmmoAmount)
{
	AmmoMap.Add(AmmoType, AmmoMap.FindRef(AmmoType) + AmmoAmount);
}

void UInventoryComponent::AddEquipment(FName EquipmentName, int EquipmentAmount)
{
	EquipmentMap.Add(EquipmentName, EquipmentMap.FindRef(EquipmentName) + EquipmentAmount);
}

void UInventoryComponent::RemoveAmmo(EAmmoType AmmoType, int AmmoAmount)
{
	AmmoMap.Add(AmmoType, AmmoMap.FindRef(AmmoType) - AmmoAmount);

	if (AmmoMap.FindRef(AmmoType) < 0) AmmoMap.Add(AmmoType, 0);
}

void UInventoryComponent::RemoveEquipment(FName EquipmentName, int EquipmentAmount)
{
	EquipmentMap.Add(EquipmentName, EquipmentMap.FindRef(EquipmentName) - EquipmentAmount);

	if (EquipmentMap.FindRef(EquipmentName) < 0) EquipmentMap.Add(EquipmentName, 0);
}





