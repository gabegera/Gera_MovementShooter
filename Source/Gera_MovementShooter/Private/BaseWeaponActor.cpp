// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeaponActor.h"

#include "BaseShooterCharacter.h"


// Sets default values
ABaseWeaponActor::ABaseWeaponActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
}

// Called when the game starts or when spawned
void ABaseWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseWeaponActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (WeaponDataTableRowHandle.IsNull() || WeaponDataTableRowHandle.RowName == NAME_None) return;
	WeaponData = *WeaponDataTableRowHandle.GetRow<FWeaponData>("");
}

// Called every frame
void ABaseWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

