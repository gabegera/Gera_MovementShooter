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

	TArray<FWeaponData*> OutRowArray;
	WeaponsDataTable->GetAllRows<FWeaponData>("", OutRowArray);
	Weapons.Reserve(9);
	
	for (int32 i = 0; i < StartingWeapons.Num(); i++)
	{
		if (StartingWeapons.FindRef(OutRowArray[i]->Name) == true)
		{
			Weapons.Add(*OutRowArray[i]);
		}
	}
}

void UInventoryComponent::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (RefreshWeaponsDataTable == true)
	{
		RefreshWeaponsDataTable = false;
	}
	
	if (WeaponsDataTable == nullptr) return;

	TArray<FWeaponData*> OutRowArray;
	WeaponsDataTable->GetAllRows<FWeaponData>("", OutRowArray);
	TMap<FName, bool> NewMap;
	
	for (int32 i = 0; i < OutRowArray.Num(); i++)
	{
		const FName Key = OutRowArray[i]->Name;
		const bool Value = StartingWeapons.FindRef(Key);
		NewMap.Add(Key, Value);
	}

	StartingWeapons.Empty();
	StartingWeapons.Append(NewMap);
}

void UInventoryComponent::PostInitProperties()
{
	Super::PostInitProperties();
}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::AddWeapon(FWeaponData NewWeapon)
{
	Weapons.Add(NewWeapon);
}

void UInventoryComponent::AddAmmo(EAmmoType AmmoType, int32 AmmoAmount)
{
	AmmoMap.Add(AmmoType, AmmoAmount);
}






