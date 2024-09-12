// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoPickupObject.h"

// Sets default values
AAmmoPickupObject::AAmmoPickupObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Change Model And Value based on Enumerator

	AmmoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ammo Mesh"), false);	
}

// Called when the game starts or when spawned
void AAmmoPickupObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAmmoPickupObject::OnConstruction(const FTransform& Transform)
{
	if (AmmoType == AmmoType::RIFLE)
	{
		AmmoMesh->SetMaterial(0, RifleMaterial);
	}
	else if (AmmoType == AmmoType::SHOTGUN)
	{
		AmmoMesh->SetMaterial(0, ShotgunMaterial);
	}
	else if (AmmoType == AmmoType::GRENADELAUNCHER)
	{
		AmmoMesh->SetMaterial(0, GrenadeLauncherMaterial);
	}
}

// Called every frame
void AAmmoPickupObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

