// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerCharacter.h"

#include "PickupObject.h"
#include "ShooterPlayerController.h"
#include "Camera/CameraComponent.h"
#include "VectorUtil.h"

// Sets default values
AShooterPlayerCharacter::AShooterPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	PerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AI Perception Stimulus"));

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	CameraComponent->bUsePawnControlRotation = true;

	GetMesh()->AttachToComponent(CameraComponent, FAttachmentTransformRules::KeepRelativeTransform);

	WeaponChildComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("WeaponChild"));
	WeaponChildComponent->SetupAttachment(GetMesh());

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

}

AShooterPlayerController* AShooterPlayerCharacter::GetPlayerController()
{
	if (PlayerController == nullptr)
	{
		PlayerController = Cast<AShooterPlayerController>(GetController());
	}

	return PlayerController;
}

// void AShooterPlayerCharacter::UpdateLook(FVector2D Input)
// {
// 	AddControllerYawInput(Input.X);
// 	AddControllerPitchInput(Input.Y);
//
// 	GEngine->AddOnScreenDebugMessage(4, 0.2f, FColor::Red, "Look Updated: " + Input.ToString());
// }

// Called when the game starts or when spawned
void AShooterPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(WeaponChildComponent) && IsValid(WeaponChildComponent->GetChildActor()->GetComponentByClass<USkeletalMeshComponent>()))
	{
		WeaponChildMesh = WeaponChildComponent->GetChildActor()->GetComponentByClass<USkeletalMeshComponent>();
	}

	if (IsValid(InventoryComponent))
	{
		if (InventoryComponent->PrimaryWeapon.IsNull() && InventoryComponent->SecondaryWeapon.IsNull() && InventoryComponent->HeavyWeapon.IsNull())
		{
			EquipWeapon(InventoryComponent->GetEmptyHands());
		}
		else if (!InventoryComponent->PrimaryWeapon.IsNull())
		{
			EquipWeapon(InventoryComponent->PrimaryWeapon);			
		}
		else if (!InventoryComponent->SecondaryWeapon.IsNull())
		{
			EquipWeapon(InventoryComponent->SecondaryWeapon);		
		}
		else if (!InventoryComponent->HeavyWeapon.IsNull())
		{
			EquipWeapon(InventoryComponent->HeavyWeapon);		
		}

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

