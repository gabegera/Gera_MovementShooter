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
	PickupMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void APickupObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void APickupObject::OnConstruction(const FTransform& Transform)
{
	switch (CurrentPickupType)
	{
	case PickupType::PISTOL_WEAPON:
		PickupMesh->SetOverlayMaterial(PistolOutlineMaterial);
		PickupMesh->SetStaticMesh(PistolMesh);
		PickupMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		break;
	case PickupType::RIFLE_WEAPON:
		PickupMesh->SetOverlayMaterial(RifleOutlineMaterial);
		PickupMesh->SetStaticMesh(RifleMesh);
		PickupMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		break;
	case PickupType::SHOTGUN_WEAPON:
		PickupMesh->SetOverlayMaterial(ShotgunOutlineMaterial);
		PickupMesh->SetStaticMesh(ShotgunMesh);
		PickupMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		break;
	case PickupType::GRENADE_LAUNCHER_WEAPON:
		PickupMesh->SetOverlayMaterial(GrenadeLauncherOutlineMaterial);
		PickupMesh->SetStaticMesh(GrenadeLauncherMesh);
		PickupMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		break;
	case PickupType::RIFLE_AMMO:
		PickupMesh->SetOverlayMaterial(RifleOutlineMaterial);
		PickupMesh->SetStaticMesh(RifleAmmoMesh);
		PickupMesh->SetRelativeRotation(FRotator(35.0f, 35.0f, 15.0f));
		break;
	case PickupType::SHOTGUN_AMMO:
		PickupMesh->SetOverlayMaterial(ShotgunOutlineMaterial);
		PickupMesh->SetStaticMesh(ShotgunAmmoMesh);
		PickupMesh->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		break;
	case PickupType::GRENADE_LAUNCHER_AMMO:
		PickupMesh->SetOverlayMaterial(GrenadeLauncherOutlineMaterial);
		PickupMesh->SetStaticMesh(GrenadeAmmoMesh);
		PickupMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		break;
	default:
		break;
	}
}

// Called every frame
void APickupObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorWorldRotation(FRotator(0.0f, 1.0f, 0.0f));
}

