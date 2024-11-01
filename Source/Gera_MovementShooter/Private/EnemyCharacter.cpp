// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "HealthComponent.h"
#include "InventoryComponent.h"
#include "PickupObject.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(FName("HealthComponent"));

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");

	WeaponChildComponent = CreateDefaultSubobject<UChildActorComponent>("Weapon");
	WeaponChildComponent->SetupAttachment(GetMesh());
	
}

void AEnemyCharacter::PickupWeapon(APickupObject* Pickup)
{
	InventoryComponent->SwapWeapons(EWeaponSlot::Primary, Pickup->GetComponentByClass<UPickupComponent>()->GetWeaponPickup());

	WeaponChildComponent->SetChildActorClass(Pickup->GetComponentByClass<UPickupComponent>()->GetWeaponPickupData().WeaponActor->GetClass());
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{	
	Super::BeginPlay();

	
	
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (HealthComponent)
	{
		HealthComponent->TakeDamage(Damage);
		
		FString DebugMessage = "Enemy Took: ";
		DebugMessage.Append(FString::SanitizeFloat(Damage));
		DebugMessage.Append(" Damage.");
		
		FString HealthMessage = "Enemy Has: ";
		HealthMessage.Append(FString::SanitizeFloat(HealthComponent->GetHealth()));
		HealthMessage.Append(" Health.");
		
		GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Red, DebugMessage);
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, HealthMessage);

		if (HealthComponent->GetHealth() <= 0.0f)
		{
			Destroy();
		}
	}
	return Damage;
}

EEnemyType AEnemyCharacter::SetEnemyType(EEnemyType NewEnemyType)
{
	EnemyType = NewEnemyType;
	
	return EnemyType;
}

