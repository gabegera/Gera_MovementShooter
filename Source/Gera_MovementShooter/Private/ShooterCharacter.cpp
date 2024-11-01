// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "PickupComponent.h"
#include "Components/CapsuleComponent.h"
#include "PickupObject.h"

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
	
}

FWeaponData AShooterCharacter::GetEquippedWeaponData()
{
	
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

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

