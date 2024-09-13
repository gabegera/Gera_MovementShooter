// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupObject.h"

// Sets default values
APickupObject::APickupObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"), false);

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
	if (CurrentPickupType == PickupType::RIFLE_AMMO)
	{
		PickupMesh->SetMaterial(0, RifleAmmoMaterial);
	}
	else if (CurrentPickupType == PickupType::SHOTGUN_AMMO)
	{
		PickupMesh->SetMaterial(0, ShotgunAmmoMaterial);
	}
	else if (CurrentPickupType == PickupType::GRENADE_LAUNCHER_AMMO)
	{
		PickupMesh->SetMaterial(0, GrenadeLauncherAmmoMaterial);
	}
}

// Called every frame
void APickupObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

