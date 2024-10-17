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
	RefreshPickup();
}

// Called every frame
void APickupObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorWorldRotation(FRotator(0.0f, 90.0f * DeltaTime, 0.0f));
}

void APickupObject::RefreshPickup()
{
	if (PickupComponent->GetPickupType() == EPickupType::Ammo)
	{
		
		switch (PickupComponent->GetAmmoType())
		{
		case EAmmoType::PistolAmmo:
			PickupComponent->SetOutlineColor(FColor::Yellow);
			PickupMesh->SetStaticMesh(PickupComponent->GetPistolAmmoMesh());
			PickupMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
			break;
		case EAmmoType::RifleAmmo:
			PickupComponent->SetOutlineColor(FColor::Blue);
			PickupMesh->SetStaticMesh(PickupComponent->GetRifleAmmoMesh());
			PickupMesh->SetRelativeRotation(FRotator(35.0f, 35.0f, 15.0f));
			break;
		case EAmmoType::ShotgunAmmo:
			PickupComponent->SetOutlineColor(FColor::Orange);
			PickupMesh->SetStaticMesh(PickupComponent->GetShotgunAmmoMesh());
			PickupMesh->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
			break;
		case EAmmoType::SniperAmmo:
			PickupComponent->SetOutlineColor(FColor::Purple);
			PickupMesh->SetStaticMesh(PickupComponent->GetSniperAmmoMesh());
			PickupMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
			break;
		case EAmmoType::ExplosiveAmmo:
			PickupComponent->SetOutlineColor(FColor::Red);
			PickupMesh->SetStaticMesh(PickupComponent->GetExplosiveAmmoMesh());
			PickupMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
			break;
		case EAmmoType::EnergyAmmo:
			PickupComponent->SetOutlineColor(FColor::Cyan);
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
			PickupComponent->SetOutlineColor(FColor::Blue);
			break;
		case EWeaponSlot::Secondary:
			PickupComponent->SetOutlineColor(FColor::Yellow);
			break;
		case EWeaponSlot::Heavy:
			PickupComponent->SetOutlineColor(FColor::Red);
			break;
		default:
			break;
		}
	}
	else if (PickupComponent->GetPickupType() == EPickupType::Equipment)
	{

		PickupMesh->SetStaticMesh(PickupComponent->GetItemPickupData().ItemMesh);
		PickupMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

		PickupComponent->SetOutlineColor(FColor::Orange);
	}
	else if (PickupComponent->GetPickupType() == EPickupType::Buff)
	{
	
		switch(PickupComponent->GetItemPickupData().BuffEffect)
		{
		case EBuffEffects::None:
			break;
		case EBuffEffects::Heal:
			PickupComponent->SetOutlineColor(FColor::Green);
			break;
		case EBuffEffects::SpeedBoost:
			PickupComponent->SetOutlineColor(FColor::Yellow);
			break;
		case EBuffEffects::DamageBoost:
			PickupComponent->SetOutlineColor(FColor::Red);
			break;
		case EBuffEffects::SlowTime:
			PickupComponent->SetOutlineColor(FColor::Purple);
			break;
		}
	
		PickupMesh->SetStaticMesh(PickupComponent->GetItemPickupData().ItemMesh);
		PickupMesh->SetRelativeRotation(FRotator(35.0f, 0.0f, 0.0f));
	}

	if (PickupComponent->GetOutlineMaterial()) PickupMesh->SetOverlayMaterial(PickupComponent->GetOutlineMaterial());
}

FDataTableRowHandle APickupObject::SetWeaponPickup(FDataTableRowHandle NewWeapon)
{
	PickupComponent->SetWeaponType(NewWeapon);
	RefreshPickup();
	return NewWeapon;
}

FDataTableRowHandle APickupObject::SetItemPickup(FDataTableRowHandle NewItem)
{
	PickupComponent->SetItemPickup(NewItem);
	RefreshPickup();
	return NewItem;
}

