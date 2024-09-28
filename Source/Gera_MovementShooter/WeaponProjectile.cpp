// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponProjectile.h"

// Sets default values
AWeaponProjectile::AWeaponProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWeaponProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponProjectile::DealDamage(AActor* Collider, AActor* PlayerCharacter, AController* PlayerController)
{
	if (Collider && IsValid(Collider->GetComponentByClass<UHealthComponent>()) && PlayerCharacter && PlayerController)
	{
		Collider->TakeDamage(200, FDamageEvent(), PlayerController, PlayerCharacter);
	}
}

// Called every frame
void AWeaponProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

