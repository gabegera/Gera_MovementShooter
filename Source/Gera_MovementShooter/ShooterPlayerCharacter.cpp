// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerCharacter.h"

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
	WeaponChildComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "weaponsocket_r");
	

}

// Called when the game starts or when spawned
void AShooterPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(WeaponChildComponent) && IsValid(WeaponChildComponent->GetChildActor()->GetComponentByClass<USkeletalMeshComponent>()))
	{
		WeaponChildMesh = WeaponChildComponent->GetChildActor()->GetComponentByClass<USkeletalMeshComponent>();
	}
	else return;
	
}

//void AShooterPlayerCharacter::Look(float InputX, float InputY)
//{
//	AddControllerYawInput(InputX);
//	AddControllerPitchInput(InputY);
//}

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

