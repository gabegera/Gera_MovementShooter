// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerCharacter.h"
#include <limits>
#include "PickupObject.h"
#include "ShooterPlayerController.h"

// Sets default values
AShooterPlayerCharacter::AShooterPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	CameraComponent->bUsePawnControlRotation = true;

	GetMesh()->AttachToComponent(CameraComponent, FAttachmentTransformRules::KeepRelativeTransform);

	WeaponChildComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("WeaponChild"));
	WeaponChildComponent->SetupAttachment(GetMesh());

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

}


// Called when the game starts or when spawned
void AShooterPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetComponentByClass<UCapsuleComponent>()->OnComponentBeginOverlap.AddDynamic(this, &AShooterPlayerCharacter::BeginOverlap);
	GetComponentByClass<UCapsuleComponent>()->OnComponentEndOverlap.AddDynamic(this, &AShooterPlayerCharacter::EndOverlap);

	if (IsValid(WeaponChildComponent) && IsValid(WeaponChildComponent->GetChildActor()->GetComponentByClass<USkeletalMeshComponent>()))
	{
		WeaponChildMesh = WeaponChildComponent->GetChildActor()->GetComponentByClass<USkeletalMeshComponent>();
	}

	if (IsValid(InventoryComponent))
	{
		EquipWeapon(InventoryComponent->PrimaryWeapon);
	}
	
}

void AShooterPlayerCharacter::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetComponentByClass<UPickupComponent>() &&
		OtherActor->GetComponentByClass<UPickupComponent>()->GetPickupType() == EPickupType::Weapon ||
		OtherActor->GetComponentByClass<UPickupComponent>()->GetPickupType() == EPickupType::Buff)
	{
		PickupSet.Add(OtherActor);
	}
}

void AShooterPlayerCharacter::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->GetComponentByClass<UPickupComponent>())
	{
		PickupSet.Remove(OtherActor);
	}
}

float AShooterPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	GEngine->AddOnScreenDebugMessage(0, 0.5f, FColor::White, "PLAYER TOOK DAMAGE");

	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (HealthComponent)
	{
		HealthComponent->TakeDamage(Damage);
	}
	return Damage;
}

AActor* AShooterPlayerCharacter::FindClosestPickup()
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

void AShooterPlayerCharacter::PickupItem()
{
	if (PickupSet.Num() <= 0) return;

	AActor* ClosestPickup = FindClosestPickup();
	UPickupComponent* PickupComponent = ClosestPickup->GetComponentByClass<UPickupComponent>();
	EPickupType ClosestPickupType = PickupComponent->GetPickupType();

	if (ClosestPickupType == EPickupType::Weapon)
	{
		FWeaponData PickupWeaponData = PickupComponent->GetWeaponPickupData();
		InventoryComponent->SwapWeapons(PickupWeaponData.WeaponSlot, PickupComponent->GetWeaponPickup());

		switch (PickupWeaponData.WeaponSlot)
		{
		case EWeaponSlot::Primary:
			EquipWeapon(InventoryComponent->PrimaryWeapon);
			break;
		case EWeaponSlot::Secondary:
			EquipWeapon(InventoryComponent->SecondaryWeapon);
			break;
		case EWeaponSlot::Heavy:
			EquipWeapon(InventoryComponent->HeavyWeapon);
			break;
		default:
			break;
		}
	}
	else if (ClosestPickupType == EPickupType::Buff)
	{
		FItemData PickupItemData = PickupComponent->GetItemPickupData();
		InventoryComponent->SwapSupportItem(PickupComponent->GetItemPickup());
	}
}

void AShooterPlayerCharacter::EquipWeapon(FDataTableRowHandle Weapon)
{
	EquippedWeapon = Weapon;

	if (auto EquippedWeaponData = EquippedWeapon.GetRow<FWeaponData>(""))
    	{
    		WeaponChildComponent->SetChildActorClass(EquippedWeaponData->WeaponActor);
    	}
}

// Called every frame
void AShooterPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AShooterPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
