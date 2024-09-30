// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupObject.h"

#include "ShooterPlayerCharacter.h"
#include "ToolBuilderUtil.h"

// Sets default values
APickupObject::APickupObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"), false);
	RootComponent->SetMobility(EComponentMobility::Movable);

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"), false);
	PickupMesh->SetupAttachment(RootComponent);
	
	WeaponSphereTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("Weapon Trigger"));
	WeaponSphereTrigger->SetupAttachment(RootComponent);
	WeaponSphereTrigger->SetSphereRadius(120.0f);

	PickupSphereTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("Pickup Trigger"));
	PickupSphereTrigger->SetupAttachment(RootComponent);
	PickupSphereTrigger->SetSphereRadius(40.0f);

	
}

// Called when the game starts or when spawned
void APickupObject::BeginPlay()
{
	Super::BeginPlay();

	PickupSphereTrigger->OnComponentBeginOverlap.AddDynamic(this, &APickupObject::BeginPickupSphereOverlap);
	WeaponSphereTrigger->OnComponentBeginOverlap.AddDynamic(this, &APickupObject::BeginWeaponSphereOverlap);
	WeaponSphereTrigger->OnComponentEndOverlap.AddDynamic(this, &APickupObject::EndWeaponSphereOverlap);
}

void APickupObject::BeginPickupSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetName().Contains("ShooterPlayerCharacter") && PickupType == EPickupType::Ammo)
	{
		OtherActor->GetComponentByClass<UInventoryComponent>()->AddAmmo(AmmoType, 20);
		Destroy();
	}
}

void APickupObject::BeginWeaponSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetName().Contains("ShooterPlayerCharacter") && PickupType == EPickupType::Weapon)
	{
		
	}
}

void APickupObject::EndWeaponSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

void APickupObject::OnConstruction(const FTransform& Transform)
{
	if (PickupType == EPickupType::Ammo)
	{
		WeaponSphereTrigger->SetVisibility(false);
		PickupSphereTrigger->SetVisibility(true);
		
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
		default:
			break;
		}		
	}
	else if (PickupType == EPickupType::Weapon)
	{
		WeaponSphereTrigger->SetVisibility(true);
		PickupSphereTrigger->SetVisibility(false);
		
		FWeaponData* WeaponData = WeaponPickup.GetRow<FWeaponData>("");

		if(!WeaponData) return;
		
		PickupMesh->SetStaticMesh(WeaponData->StaticMesh);
		PickupMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		//WeaponSphereTrigger->SetSphereRadius(PickupMesh->Bounds.GetSphere().W * 2);

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

