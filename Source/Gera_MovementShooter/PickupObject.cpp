// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupObject.h"

// Sets default values
APickupObject::APickupObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"), false);
	RootComponent->SetMobility(EComponentMobility::Movable);

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"), false);
	PickupMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APickupObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void APickupObject::OnConstruction(const FTransform& Transform)
{
	if (PickupType == EPickupType::Ammo)
	{
		switch (AmmoType)
		{
		case EAmmoType::PistolAmmo:
			PickupMesh->SetOverlayMaterial(SecondaryOutlineMaterial);
			PickupMesh->SetStaticMesh(PistolAmmoMesh);
			PickupMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
			break;
		case EAmmoType::RifleAmmo:
			PickupMesh->SetOverlayMaterial(PrimaryOutlineMaterial);
			PickupMesh->SetStaticMesh(RifleAmmoMesh);
			PickupMesh->SetRelativeRotation(FRotator(35.0f, 35.0f, 15.0f));
			break;
		case EAmmoType::ShotgunAmmo:
			PickupMesh->SetOverlayMaterial(PrimaryOutlineMaterial);
			PickupMesh->SetStaticMesh(ShotgunAmmoMesh);
			PickupMesh->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
			break;
		case EAmmoType::SniperAmmo:
			PickupMesh->SetOverlayMaterial(HeavyOutlineMaterial);
			PickupMesh->SetStaticMesh(SniperAmmoMesh);
			PickupMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
			break;
		case EAmmoType::ExplosiveAmmo:
			PickupMesh->SetOverlayMaterial(HeavyOutlineMaterial);
			PickupMesh->SetStaticMesh(ExplosiveAmmoMesh);
			PickupMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
			break;
		default: ;
		}		
	}
	else if (PickupType == EPickupType::Weapon)
	{
		FWeaponData* WeaponData = WeaponPickup.GetRow<FWeaponData>("");

		if(!WeaponData) return;
		
		PickupMesh->SetStaticMesh(WeaponData->StaticMesh);
		PickupMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

		switch (WeaponData->WeaponSlot)
		{
		case EWeaponSlot::Primary:
			PickupMesh->SetOverlayMaterial(PrimaryOutlineMaterial);
			break;
		case EWeaponSlot::Secondary:
			PickupMesh->SetOverlayMaterial(SecondaryOutlineMaterial);
			break;
		case EWeaponSlot::Heavy:
			PickupMesh->SetOverlayMaterial(HeavyOutlineMaterial);
			break;
		default:
			break;
		}
	}
}

// Called every frame
void APickupObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorWorldRotation(FRotator(0.0f, 90.0f * DeltaTime, 0.0f));
}

