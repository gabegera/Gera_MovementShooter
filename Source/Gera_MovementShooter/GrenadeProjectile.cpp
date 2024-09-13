// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadeProjectile.h"

// Sets default values
AGrenadeProjectile::AGrenadeProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"), false);

	GrenadeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Grenade Mesh"), false);
	GrenadeMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AGrenadeProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGrenadeProjectile::DealDamage(AActor* Collider, AActor* PlayerCharacter, AController* PlayerController)
{
	if (Collider && IsValid(Collider->GetComponentByClass<UHealthComponent>()) && PlayerCharacter && PlayerController)
	{
		Collider->TakeDamage(MaxDamage, FDamageEvent(), PlayerController, PlayerCharacter);
	}
}

// Called every frame
void AGrenadeProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

