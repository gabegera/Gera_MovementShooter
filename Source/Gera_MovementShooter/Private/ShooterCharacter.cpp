// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"

#include "ExplosiveComponent.h"
#include "PickupComponent.h"
#include "Components/CapsuleComponent.h"
#include "PickupObject.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetComponentByClass<UCapsuleComponent>()->OnComponentBeginOverlap.AddDynamic(this, &AShooterCharacter::BeginOverlap);
	GetComponentByClass<UCapsuleComponent>()->OnComponentEndOverlap.AddDynamic(this, &AShooterCharacter::EndOverlap);

	//if (!InventoryComponent->PrimaryWeapon.IsNull()) EquipWeapon(InventoryComponent->PrimaryWeapon);
}

void AShooterCharacter::OnConstruction(const FTransform& Transform)
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

FWeaponData AShooterCharacter::GetEquippedWeaponData()
{
	if (EquippedWeapon.IsNull()) EquipWeapon(InventoryComponent->PrimaryWeapon);
	
	return *EquippedWeapon.GetRow<FWeaponData>("");
}

void AShooterCharacter::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetComponentByClass<UPickupComponent>() &&
		OtherActor->GetComponentByClass<UPickupComponent>()->GetPickupType() == EPickupType::Weapon ||
		OtherActor->GetComponentByClass<UPickupComponent>()->GetPickupType() == EPickupType::Buff)
	{
		PickupSet.Add(OtherActor);
	}
}

void AShooterCharacter::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->GetComponentByClass<UPickupComponent>())
	{
		PickupSet.Remove(OtherActor);
	}
}

float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	GEngine->AddOnScreenDebugMessage(0, 0.5f, FColor::White, "PLAYER TOOK DAMAGE");

	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (HealthComponent)
	{
		HealthComponent->TakeDamage(Damage);
	}
	return Damage;
}

AActor* AShooterCharacter::FindClosestPickup()
{
	if (PickupSet.Num() <= 0) return nullptr;
	
	TMap<float, AActor*> PickupsMap;
	
	float DistanceFromPickup = std::numeric_limits<float>::max();
	for (AActor* Pickup : PickupSet)
	{
		PickupsMap.Add(GetDistanceTo(Pickup), Pickup);
		if (GetDistanceTo(Pickup) < DistanceFromPickup) DistanceFromPickup = GetDistanceTo(Pickup);
	}

	return PickupsMap[DistanceFromPickup];
}

void AShooterCharacter::PickupItem()
{
	if (PickupSet.Num() <= 0) return;

	AActor* ClosestPickup = FindClosestPickup();
	UPickupComponent* PickupComponent = ClosestPickup->GetComponentByClass<UPickupComponent>();
	EPickupType ClosestPickupType = PickupComponent->GetPickupType();

	if (ClosestPickupType == EPickupType::Weapon)
	{
		FWeaponData PickupWeaponData = PickupComponent->GetWeaponPickupData();
		FDataTableRowHandle NewWeapon = PickupComponent->GetWeaponPickup();
		
		switch (PickupWeaponData.WeaponSlot)
		{
		case EWeaponSlot::Primary:
			EquipWeapon(PickupComponent->GetWeaponPickup());
			if (InventoryComponent->PrimaryWeapon.IsNull())
			{
				Cast<APickupObject>(ClosestPickup)->Destroy();
			}
			else Cast<APickupObject>(ClosestPickup)->SetWeaponPickup(InventoryComponent->PrimaryWeapon);
			break;
			
		case EWeaponSlot::Secondary:
			EquipWeapon(PickupComponent->GetWeaponPickup());
			if (InventoryComponent->SecondaryWeapon.IsNull())
			{
				Cast<APickupObject>(ClosestPickup)->Destroy();
			}
			else Cast<APickupObject>(ClosestPickup)->SetWeaponPickup(InventoryComponent->SecondaryWeapon);
			break;
			
		case EWeaponSlot::Heavy:
			EquipWeapon(PickupComponent->GetWeaponPickup());
			if (InventoryComponent->HeavyWeapon.IsNull())
			{
				Cast<APickupObject>(ClosestPickup)->Destroy();
			}
			else Cast<APickupObject>(ClosestPickup)->SetWeaponPickup(InventoryComponent->HeavyWeapon);
			break;
			
		default:
			break;
		}
		if (!NewWeapon.IsNull())
		{
			InventoryComponent->SwapWeapons(PickupWeaponData.WeaponSlot, NewWeapon);			
		}

	}
	else if (ClosestPickupType == EPickupType::Buff)
	{
		FItemData PickupItemData = PickupComponent->GetItemPickupData();
		
		if (InventoryComponent->SupportItemSlot.IsNull())
		{
			InventoryComponent->SwapSupportItem(PickupComponent->GetItemPickup());
			ClosestPickup->Destroy();
		}
		else
		{
			FDataTableRowHandle NewItem = PickupComponent->GetItemPickup();
			Cast<APickupObject>(ClosestPickup)->SetItemPickup(InventoryComponent->SupportItemSlot);
			InventoryComponent->SwapSupportItem(NewItem);
		}
		
		
	}
}

void AShooterCharacter::EquipWeapon(FDataTableRowHandle NewWeapon)
{
	if (EquippedWeapon == NewWeapon) return;
	if (NewWeapon.IsNull()) return;
	
	EquippedWeapon = NewWeapon;

	if (auto EquippedWeaponData = EquippedWeapon.GetRow<FWeaponData>(""))
    	{
    		WeaponChildComponent->SetChildActorClass(EquippedWeaponData->WeaponActor);
    	}
}

void AShooterCharacter::ShootHitscan(float WeaponSpreadInDegrees, const FVector ShotOrigin, FVector ShotTarget, float Damage)
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

void AShooterCharacter::ShootProjectile(const TSubclassOf<AActor> ProjectileActor, const float WeaponSpreadInDegrees, const FVector ShotOrigin, const FVector ShotTarget, const float ProjectileVelocity, const float Damage)
{
	AActor* SpawnedProjectile = GetWorld()->SpawnActor(ProjectileActor);
	
	SpawnedProjectile->SetActorLocation(ShotOrigin);
	
	FVector ShotDirection = UKismetMathLibrary::GetDirectionUnitVector(ShotOrigin, ShotTarget);
	ShotDirection = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(ShotDirection, WeaponSpreadInDegrees / 2);
	const FVector NewVelocity = ShotDirection * ProjectileVelocity;
	
	SpawnedProjectile->GetComponentByClass<UProjectileMovementComponent>()->Velocity = NewVelocity;
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

