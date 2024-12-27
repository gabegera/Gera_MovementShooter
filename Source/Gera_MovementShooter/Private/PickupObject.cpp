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
	switch (GetPickupType())
	{
	case EPickupType::Weapon:
		break;
	case EPickupType::Ammo:
		if (Execute_PickupConsumable(OtherActor, GetConsumableEffect(), GetPickupData().PickupAmount))
		{
			Destroy();
		}
		break;
	case EPickupType::Ability:
		break;
	case EPickupType::Consumable:
		if (Execute_PickupAmmo(OtherActor, GetAmmoType(), GetPickupData().PickupAmount))
		{
			Destroy();
		}
		break;
	case EPickupType::Equipment:
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
	PickupMesh->SetOverlayMaterial(DynamicOutline);
}

void APickupObject::RefreshPickup()
{
	if (PickupDataTableRowHandle.IsNull() || PickupDataTableRowHandle.GetRow<FPickupData>("") == nullptr) return;
	
	SetOutlineColor(GetOutlineColor());
	if (GetPickupMesh()) PickupMesh->SetStaticMesh(GetPickupMesh());
	PickupMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	
	if (GetPickupType() == EPickupType::Ammo)
	{
		switch (GetAmmoType())
		{
		case EAmmoType::RifleAmmo:
			PickupMesh->SetRelativeRotation(FRotator(35.0f, 35.0f, 15.0f));
			break;
			
		case EAmmoType::ShotgunAmmo:
			PickupMesh->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
			break;
		default:
			break;
		}		
	}
	else if (GetPickupType() == EPickupType::Weapon)
	{
		if (!GetWeaponDataTableRowHandle().IsNull() && GetWeaponDataTableRowHandle().GetRow<FWeaponData>("")->StaticMesh)
		{
			PickupMesh->SetStaticMesh(GetWeaponDataTableRowHandle().GetRow<FWeaponData>("")->StaticMesh);
		}
		PickupMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		//InteractSphereTrigger->SetSphereRadius(PickupMesh->Bounds.GetSphere().W * 2);
	}
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

