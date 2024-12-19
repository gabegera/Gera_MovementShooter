// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupObject.h"
#include "ShooterPlayerCharacter.h"

// Sets default values
APickupObject::APickupObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
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
	if (OtherActor->GetName().Contains("ShooterPlayerCharacter") && GetPickupType() == EPickupType::Ammo)
	{
		OtherActor->GetComponentByClass<UInventoryComponent>()->AddAmmo(GetAmmoType(), 20);
		Destroy();
	}
	else if (OtherActor->GetName().Contains("ShooterPlayerCharacter") && GetPickupType() == EPickupType::Equipment)
	{
		OtherActor->GetComponentByClass<UInventoryComponent>()->AddEquipment(GetItemPickupData().Name, 1);
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

void APickupObject::SetOutlineColor(FLinearColor Color)
{
	if (!IsValid(OutlineMaterial)) return;
	
	if (!IsValid(DynamicOutline))
	{
		DynamicOutline = UMaterialInstanceDynamic::Create(OutlineMaterial, this);
	}
	
	DynamicOutline->SetVectorParameterValue(FName("Color"), Color);
}

void APickupObject::RefreshPickup()
{
	if (GetPickupType() == EPickupType::Ammo)
	{
		
		switch (GetAmmoType())
		{
		case EAmmoType::PistolAmmo:
			SetOutlineColor(FColor::Yellow);
			PickupMesh->SetStaticMesh(GetPistolAmmoMesh());
			PickupMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
			break;
		case EAmmoType::RifleAmmo:
			SetOutlineColor(FColor::Blue);
			PickupMesh->SetStaticMesh(GetRifleAmmoMesh());
			PickupMesh->SetRelativeRotation(FRotator(35.0f, 35.0f, 15.0f));
			break;
		case EAmmoType::ShotgunAmmo:
			SetOutlineColor(FColor::Orange);
			PickupMesh->SetStaticMesh(GetShotgunAmmoMesh());
			PickupMesh->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
			break;
		case EAmmoType::SniperAmmo:
			SetOutlineColor(FColor::Purple);
			PickupMesh->SetStaticMesh(GetSniperAmmoMesh());
			PickupMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
			break;
		case EAmmoType::ExplosiveAmmo:
			SetOutlineColor(FColor::Red);
			PickupMesh->SetStaticMesh(GetExplosiveAmmoMesh());
			PickupMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
			break;
		case EAmmoType::EnergyAmmo:
			SetOutlineColor(FColor::Cyan);
			PickupMesh->SetStaticMesh(GetEnergyAmmoMesh());
			PickupMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
			break;			
		case EAmmoType::None:
			break;
		default:
			break;
		}		
	}
	else if (GetPickupType() == EPickupType::Weapon)
	{
		
		FWeaponData* WeaponData = GetWeaponPickup().GetRow<FWeaponData>("");

		if(!WeaponData) return;
		
		PickupMesh->SetStaticMesh(WeaponData->StaticMesh);
		PickupMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		//InteractSphereTrigger->SetSphereRadius(PickupMesh->Bounds.GetSphere().W * 2);

		SetOutlineColor(GetWeaponPickupData().OutlineColor);
	}
	else if (GetPickupType() == EPickupType::Equipment)
	{

		PickupMesh->SetStaticMesh(GetItemPickupData().ItemMesh);
		PickupMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		SetOutlineColor(GetItemPickupData().OutlineColor);
	}
	else if (GetPickupType() == EPickupType::Buff)
	{
		PickupMesh->SetStaticMesh(GetItemPickupData().ItemMesh);
		SetOutlineColor(GetItemPickupData().OutlineColor);
		PickupMesh->SetRelativeRotation(FRotator(35.0f, 0.0f, 0.0f));
	}

	if (GetOutlineMaterial()) PickupMesh->SetOverlayMaterial(GetOutlineMaterial());
}

FDataTableRowHandle APickupObject::SetWeaponPickup(FDataTableRowHandle NewWeapon)
{
	WeaponPickup = NewWeapon;
	RefreshPickup();
	return WeaponPickup;
}

FDataTableRowHandle APickupObject::SetItemPickup(FDataTableRowHandle NewItem)
{
	ItemPickup = NewItem;
	RefreshPickup();
	return ItemPickup;
}

