// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseShooterCharacter.h"

#include "ExplosiveComponent.h"
#include "Components/CapsuleComponent.h"
#include "PickupObject.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABaseShooterCharacter::ABaseShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetComponentByClass<UCapsuleComponent>()->OnComponentBeginOverlap.AddDynamic(this, &ABaseShooterCharacter::BeginOverlap);
	GetComponentByClass<UCapsuleComponent>()->OnComponentEndOverlap.AddDynamic(this, &ABaseShooterCharacter::EndOverlap);

	//if (!InventoryComponent->PrimaryWeapon.IsNull()) EquipWeapon(InventoryComponent->PrimaryWeapon);
}

void ABaseShooterCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	EquipWeapon(InventoryComponent->PrimaryWeapon);

	// if (InventoryComponent->PrimaryWeapon.IsNull()) {
	// 	EquipWeapon(InventoryComponent->PrimaryWeapon);
	// }
	// else if (InventoryComponent->SecondaryWeapon.IsNull()) {
	// 	EquipWeapon(InventoryComponent->SecondaryWeapon);
	// }
	// else if (InventoryComponent->HeavyWeapon.IsNull()){
	// 	EquipWeapon(InventoryComponent->HeavyWeapon);
	// }
	// else
	// {
	// 	WeaponChildComponent->DestroyChildActor();
	// }
	
}

FWeaponData ABaseShooterCharacter::GetEquippedWeaponData()
{
	if (EquippedWeapon.IsNull()) EquipWeapon(InventoryComponent->PrimaryWeapon);
	
	return *EquippedWeapon.GetRow<FWeaponData>("");
}

void ABaseShooterCharacter::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass() == APickupObject::StaticClass())
	{
		APickupObject* OverlappedPickup = Cast<APickupObject>(OtherActor);

		if (OverlappedPickup->GetPickupType() != EPickupType::Ammo &&
			OverlappedPickup->GetPickupType() != EPickupType::Equipment)
		{
			PickupSet.Add(OverlappedPickup);
		}
	}
}

void ABaseShooterCharacter::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->GetClass() == APickupObject::StaticClass())
	{
		PickupSet.Remove(Cast<APickupObject>(OtherActor));
	}
}

float ABaseShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	GEngine->AddOnScreenDebugMessage(0, 0.5f, FColor::White, "PLAYER TOOK DAMAGE");

	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (HealthComponent)
	{
		HealthComponent->TakeDamage(Damage);
	}
	return Damage;
}

APickupObject* ABaseShooterCharacter::FindClosestPickup()
{
	if (PickupSet.Num() <= 0) return nullptr;
	
	TMap<float, APickupObject*> PickupsMap;
	
	float DistanceFromPickup = std::numeric_limits<float>::max();
	for (APickupObject* Pickup : PickupSet)
	{
		PickupsMap.Add(GetDistanceTo(Pickup), Pickup);
		if (GetDistanceTo(Pickup) < DistanceFromPickup) DistanceFromPickup = GetDistanceTo(Pickup);
	}

	return PickupsMap[DistanceFromPickup];
}

// void ABaseShooterCharacter::PickupItem()
// {
// 	if (PickupSet.Num() <= 0) return;
//
// 	APickupObject* ClosestPickup = FindClosestPickup();
// 	EPickupType ClosestPickupType = ClosestPickup->GetPickupType();
//
// 	if (ClosestPickupType == EPickupType::Weapon)
// 	{
// 		FWeaponData PickupWeaponData = ClosestPickup->GetWeaponPickupData();
// 		FDataTableRowHandle NewWeapon = ClosestPickup->GetWeaponPickup();
// 		
// 		switch (PickupWeaponData.WeaponSlot)
// 		{
// 		case EWeaponSlot::Primary:
// 			EquipWeapon(ClosestPickup->GetWeaponPickup());
// 			if (InventoryComponent->PrimaryWeapon.IsNull())
// 			{
// 				Cast<APickupObject>(ClosestPickup)->Destroy();
// 			}
// 			else Cast<APickupObject>(ClosestPickup)->SetWeaponPickup(InventoryComponent->PrimaryWeapon);
// 			break;
// 			
// 		case EWeaponSlot::Secondary:
// 			EquipWeapon(ClosestPickup->GetWeaponPickup());
// 			if (InventoryComponent->SecondaryWeapon.IsNull())
// 			{
// 				Cast<APickupObject>(ClosestPickup)->Destroy();
// 			}
// 			else Cast<APickupObject>(ClosestPickup)->SetWeaponPickup(InventoryComponent->SecondaryWeapon);
// 			break;
// 			
// 		case EWeaponSlot::Heavy:
// 			EquipWeapon(ClosestPickup->GetWeaponPickup());
// 			if (InventoryComponent->HeavyWeapon.IsNull())
// 			{
// 				Cast<APickupObject>(ClosestPickup)->Destroy();
// 			}
// 			else Cast<APickupObject>(ClosestPickup)->SetWeaponPickup(InventoryComponent->HeavyWeapon);
// 			break;
// 			
// 		default:
// 			break;
// 		}
// 		if (!NewWeapon.IsNull())
// 		{
// 			InventoryComponent->SwapWeapons(PickupWeaponData.WeaponSlot, NewWeapon);			
// 		}
//
// 	}
// 	else if (ClosestPickupType == EPickupType::Buff)
// 	{
// 		FItemData PickupItemData = ClosestPickup->GetItemPickupData();
// 		
// 		if (InventoryComponent->SupportItemSlot.IsNull())
// 		{
// 			InventoryComponent->SwapSupportItem(ClosestPickup->GetItemPickup());
// 			ClosestPickup->Destroy();
// 		}
// 		else
// 		{
// 			FDataTableRowHandle NewItem = ClosestPickup->GetItemPickup();
// 			Cast<APickupObject>(ClosestPickup)->SetItemPickup(InventoryComponent->SupportItemSlot);
// 			InventoryComponent->SwapSupportItem(NewItem);
// 		}
// 		
// 		
// 	}
// }

void ABaseShooterCharacter::EquipWeapon(FDataTableRowHandle NewWeapon)
{
	if (EquippedWeapon == NewWeapon) return;
	if (NewWeapon.IsNull()) return;
	
	EquippedWeapon = NewWeapon;

	if (auto EquippedWeaponData = EquippedWeapon.GetRow<FWeaponData>(""))
    	{
    		WeaponChildComponent->SetChildActorClass(EquippedWeaponData->WeaponActor);
    	}
}

void ABaseShooterCharacter::ShootHitscan(float WeaponSpreadInDegrees, const FVector ShotOrigin, FVector ShotTarget, float Damage)
{
	FHitResult Hit;
	FCollisionQueryParams IgnorePlayer;
	IgnorePlayer.AddIgnoredActor(GetUniqueID());
	const FVector ShotDirection = UKismetMathLibrary::GetDirectionUnitVector(ShotOrigin, ShotTarget);
	const float ShotLength = (ShotTarget - ShotOrigin).Length();
	ShotTarget = ShotOrigin + UKismetMathLibrary::RandomUnitVectorInConeInDegrees(ShotDirection, WeaponSpreadInDegrees / 2) * ShotLength + GetActorForwardVector() * 20.0f;
	
	DrawDebugLine(GetWorld(), ShotOrigin, ShotTarget, FColor::Red, false, 0.2f, 0, 0.5f);
	GetWorld()->LineTraceSingleByChannel(Hit, ShotOrigin, ShotTarget, ECC_PhysicsBody, IgnorePlayer);
	
	AActor* ActorHit = Hit.GetActor();

	// If Hit has Health, Deal Damage
	if (ActorHit && IsValid(ActorHit->GetComponentByClass<UHealthComponent>()))
	{
		GEngine->AddOnScreenDebugMessage(0, 0.5f, FColor::White, "HIT ACTOR");
		ActorHit->TakeDamage(Damage, FDamageEvent(), GetController(), this);
	}
	// Add Impulse to Hit
	if (ActorHit && IsValid(ActorHit->GetComponentByClass<UStaticMeshComponent>()))
	{
		ActorHit->GetComponentByClass<UStaticMeshComponent>()->AddRadialImpulse(Hit.ImpactPoint, 128, 50 * Damage, ERadialImpulseFalloff(), true);
	}
	// If Hit is Explosive, blow up Hit
	if (ActorHit && IsValid(ActorHit->GetComponentByClass<UExplosiveComponent>()))
	{
		ActorHit->GetComponentByClass<UExplosiveComponent>()->Explode();
	}
}

void ABaseShooterCharacter::ShootProjectile(const TSubclassOf<AActor> ProjectileActor, const float WeaponSpreadInDegrees, const FVector ShotOrigin, const FVector ShotTarget, const float ProjectileVelocity, const float Damage)
{
	AActor* SpawnedProjectile = GetWorld()->SpawnActor(ProjectileActor);
	
	SpawnedProjectile->SetActorLocation(ShotOrigin);
	
	FVector ShotDirection = UKismetMathLibrary::GetDirectionUnitVector(ShotOrigin, ShotTarget);
	ShotDirection = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(ShotDirection, WeaponSpreadInDegrees / 2);
	const FVector NewVelocity = ShotDirection * ProjectileVelocity;
	
	SpawnedProjectile->GetComponentByClass<UProjectileMovementComponent>()->Velocity = NewVelocity;
}

bool ABaseShooterCharacter::PickupAmmo_Implementation(const EAmmoType AmmoType, const int32 AmmoAmount)
{
	InventoryComponent->AddAmmo(AmmoType, AmmoAmount);
	
	return true;
}

bool ABaseShooterCharacter::PickupWeapon_Implementation(FDataTableRowHandle WeaponDataTableRowHandle)
{
	FWeaponData* WeaponData = WeaponDataTableRowHandle.GetRow<FWeaponData>("");

	InventoryComponent->SwapWeapons(WeaponData->WeaponSlot, WeaponDataTableRowHandle);
	
	return true;
}

bool ABaseShooterCharacter::PickupConsumable_Implementation(const EConsumableEffect ConsumableEffect, const int32 ConsumableAmount)
{
	switch (ConsumableEffect)
	{
	case EConsumableEffect::Healing:
		HealthComponent->AddHealth(ConsumableAmount);
		break;
	}
	
	return true;
}

// Called every frame
void ABaseShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

