// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupObject.h"
#include "ShooterPlayerCharacter.h"
#include "DataTables/AmmoData.h"

// Sets default values
APickupObject::APickupObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	PickupStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Static Mesh"));
	PickupStaticMesh->SetupAttachment(RootComponent);

	PickupSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Pickup Skeletal Mesh"));
	PickupSkeletalMesh->SetupAttachment(RootComponent);
	
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
	switch (PickupType)
	{
	
	case EPickupType::Ammo:
		if (Execute_PickupAmmo(OtherActor, GetAmmoData().AmmoType, GetAmmoData().PickupAmount))
		{
			Destroy();
		}
		break;
	case EPickupType::Consumable:
		if (Execute_PickupConsumable(OtherActor, GetConsumableEffect(), GetConsumableData().PickupAmount))
		{
			Destroy();
		}
		break;
	case EPickupType::Equipment:
		if (Execute_PickupEquipment(OtherActor, GetEquipmentData()))
		{
			Destroy();
		}
		break;
	case EPickupType::Weapon:
		if (Execute_PickupWeapon(OtherActor, GetWeaponData()))
		{
			Destroy();
		}
		break;	
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
	if (!IsValid(OutlineMaterialInstance)) return;

	UMaterialInstanceDynamic* DynamicOutline = UMaterialInstanceDynamic::Create(OutlineMaterialInstance, this);
	
	DynamicOutline->SetVectorParameterValue(FName("Color"), Color);
	PickupStaticMesh->SetOverlayMaterial(DynamicOutline);
}

void APickupObject::RefreshPickup()
{
	UStaticMesh* PickupMesh = nullptr;
	FColor PickupOutlineColor = FColor::Black;
	
	switch (PickupType)
	{
	case EPickupType::Ammo:
		if (AmmoDataTableRowHandle.IsNull()) return;
		PickupMesh = GetAmmoData().AmmoMesh;
		PickupOutlineColor = GetAmmoData().PickupOutlineColor;
		break;
	case EPickupType::Consumable:
		if (ConsumableDataTableRowHandle.IsNull()) return;
		PickupMesh = GetConsumableData().ConsumableMesh;
		PickupOutlineColor = GetConsumableData().PickupOutlineColor;
		break;
	case EPickupType::Equipment:
		if (EquipmentDataTableRowHandle.IsNull()) return;
		PickupMesh = GetEquipmentData().EquipmentMesh;
		PickupOutlineColor = GetEquipmentData().PickupOutlineColor;
		break;
	case EPickupType::Weapon:
		if (WeaponDataTableRowHandle.IsNull()) return;
		PickupMesh = GetWeaponData().StaticMesh;
		PickupOutlineColor = GetWeaponData().PickupOutlineColor;
		break;
	}
	
	SetOutlineColor(PickupOutlineColor);
	PickupStaticMesh->SetStaticMesh(PickupMesh);
	PickupStaticMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
}

// FDataTableRowHandle APickupObject::SetWeaponPickup(FDataTableRowHandle NewWeapon)
// {
// 	WeaponPickup = NewWeapon;
// 	RefreshPickup();
// 	return WeaponPickup;
// }
//
// FDataTableRowHandle APickupObject::SetItemPickup(FDataTableRowHandle NewItem)
// {
// 	ItemPickup = NewItem;
// 	RefreshPickup();
// 	return ItemPickup;
// }

