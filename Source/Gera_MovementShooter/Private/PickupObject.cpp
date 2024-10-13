// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupObject.h"
#include "ShooterPlayerCharacter.h"

// Sets default values
APickupObject::APickupObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent->SetMobility(EComponentMobility::Movable);

	PickupComponent = CreateDefaultSubobject<UPickupComponent>(TEXT("Pickup Component"));
	
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"));
	PickupMesh->SetupAttachment(RootComponent);
	
	InteractSphereTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("Weapon Trigger"));
	InteractSphereTrigger->SetupAttachment(RootComponent);
	InteractSphereTrigger->SetSphereRadius(120.0f);
	InteractSphereTrigger->SetAutoActivate(true);
	

	PickupSphereTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("Pickup Trigger"));
	PickupSphereTrigger->SetupAttachment(RootComponent);
	PickupSphereTrigger->SetSphereRadius(40.0f);
	PickupSphereTrigger->SetAutoActivate(true);

	
}

// Called when the game starts or when spawned
void APickupObject::BeginPlay()
{
	Super::BeginPlay();

	PickupSphereTrigger->OnComponentBeginOverlap.AddDynamic(this, &APickupObject::BeginPickupSphereOverlap);
}

void APickupObject::BeginPickupSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetName().Contains("ShooterPlayerCharacter") && PickupComponent->GetPickupType() == EPickupType::Ammo)
	{
		OtherActor->GetComponentByClass<UInventoryComponent>()->AddAmmo(PickupComponent->GetAmmoType(), 20);
		Destroy();
	}
	else if (OtherActor->GetName().Contains("ShooterPlayerCharacter") && PickupComponent->GetPickupType() == EPickupType::Equipment)
	{
		OtherActor->GetComponentByClass<UInventoryComponent>()->AddEquipment(PickupComponent->GetItemPickupData().Name, 1);
		Destroy();
	}
}

void APickupObject::OnConstruction(const FTransform& Transform)
{
	
	if (PickupComponent->GetPickupType() == EPickupType::Ammo)
	{
		
		switch (PickupComponent->GetAmmoType())
		{
		case EAmmoType::PistolAmmo:
			PickupMesh->SetOverlayMaterial(PickupComponent->GetSecondaryOutlineMaterial());
			PickupMesh->SetStaticMesh(PickupComponent->GetPistolAmmoMesh());
			PickupMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
			break;
		case EAmmoType::RifleAmmo:
			PickupMesh->SetOverlayMaterial(PickupComponent->GetPrimaryOutlineMaterial());
			PickupMesh->SetStaticMesh(PickupComponent->GetRifleAmmoMesh());
			PickupMesh->SetRelativeRotation(FRotator(35.0f, 35.0f, 15.0f));
			break;
		case EAmmoType::ShotgunAmmo:
			PickupMesh->SetOverlayMaterial(PickupComponent->GetPrimaryOutlineMaterial());
			PickupMesh->SetStaticMesh(PickupComponent->GetShotgunAmmoMesh());
			PickupMesh->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
			break;
		case EAmmoType::SniperAmmo:
			PickupMesh->SetOverlayMaterial(PickupComponent->GetHeavyOutlineMaterial());
			PickupMesh->SetStaticMesh(PickupComponent->GetSniperAmmoMesh());
			PickupMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
			break;
		case EAmmoType::ExplosiveAmmo:
			PickupMesh->SetOverlayMaterial(PickupComponent->GetHeavyOutlineMaterial());
			PickupMesh->SetStaticMesh(PickupComponent->GetExplosiveAmmoMesh());
			PickupMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
			break;
		case EAmmoType::EnergyAmmo:
			PickupMesh->SetOverlayMaterial(PickupComponent->GetSecondaryOutlineMaterial());
			PickupMesh->SetStaticMesh(PickupComponent->GetEnergyAmmoMesh());
			PickupMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
			break;			
		case EAmmoType::None:
			break;
		default:
			break;
		}		
	}
	else if (PickupComponent->GetPickupType() == EPickupType::Weapon)
	{
		
		FWeaponData* WeaponData = PickupComponent->GetWeaponPickup().GetRow<FWeaponData>("");

		if(!WeaponData) return;
		
		PickupMesh->SetStaticMesh(WeaponData->StaticMesh);
		PickupMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		//InteractSphereTrigger->SetSphereRadius(PickupMesh->Bounds.GetSphere().W * 2);

		switch (WeaponData->WeaponSlot)
		{
		case EWeaponSlot::Primary:
			PickupMesh->SetOverlayMaterial(PickupComponent->GetPrimaryOutlineMaterial());
			break;
		case EWeaponSlot::Secondary:
			PickupMesh->SetOverlayMaterial(PickupComponent->GetSecondaryOutlineMaterial());
			break;
		case EWeaponSlot::Heavy:
			PickupMesh->SetOverlayMaterial(PickupComponent->GetHeavyOutlineMaterial());
			break;
		default:
			break;
		}
	}
	else if (PickupComponent->GetPickupType() == EPickupType::Equipment)
	{

		PickupMesh->SetStaticMesh(PickupComponent->GetItemPickupData().ItemMesh);
		PickupMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

		PickupMesh->SetOverlayMaterial(PickupComponent->GetEquipmentOutlineMaterial());
	}
	else if (PickupComponent->GetPickupType() == EPickupType::Buff)
	{

		PickupMesh->SetStaticMesh(PickupComponent->GetItemPickupData().ItemMesh);
		PickupMesh->SetRelativeRotation(FRotator(35.0f, 0.0f, 0.0f));

		PickupMesh->SetOverlayMaterial(PickupComponent->GetBuffOutlineMaterial());
	}
}

// Called every frame
void APickupObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorWorldRotation(FRotator(0.0f, 90.0f * DeltaTime, 0.0f));
}

