// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"


void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = GetPawn<AShooterPlayerCharacter>();

	// Get the Enhanced Input Local Player Subsystem from the Local Player related to our Player Controller.
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// PawnClientRestart can run more than once in an Actor's lifetime, so start by clearing out any leftover mappings.
		Subsystem->ClearAllMappings();

		// Add each mapping context, along with their priority values. Higher values outprioritize lower values.
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AShooterPlayerController::Move(float InputX, float InputY)
{
	if (!IsValid(PlayerCharacter)) return;

	PlayerCharacter->AddMovementInput(PlayerCharacter->GetActorRightVector(), InputX);
	PlayerCharacter->AddMovementInput(PlayerCharacter->GetActorForwardVector(), InputY);
}

void AShooterPlayerController::Look(float InputX, float InputY)
{
	if (!IsValid(PlayerCharacter)) return;

	PlayerCharacter->AddControllerYawInput(InputX * MouseSensitivity);
	PlayerCharacter->AddControllerPitchInput(InputY * MouseSensitivity);
}

void AShooterPlayerController::Jump()
{
	if (!IsValid(PlayerCharacter)) return;
	
	PlayerCharacter->Jump();
}

void AShooterPlayerController::Dash(float InputX, float InputY)
{
	if (!IsValid(PlayerCharacter)) return;

	if (DashCooldown <= 0.0f && !PlayerCharacter->bIsCrouched)
	{
		FVector DashDirection;
		if (InputX == 0 && InputY == 0)
		{
			DashDirection = PlayerCharacter->GetActorForwardVector();
		}
		DashDirection = PlayerCharacter->GetActorRightVector() * InputX + PlayerCharacter->GetActorForwardVector() * InputY;
		
		PlayerCharacter->LaunchCharacter(DashDirection * DashVelocity, false, false);
		DashCooldown = DashTimer;
	}
	
}

void AShooterPlayerController::Crouch()
{
	PlayerCharacter->Crouch();
}

void AShooterPlayerController::StopCrouch()
{
	PlayerCharacter->UnCrouch();
}

void AShooterPlayerController::ShootHitscan(float SpreadX, float SpreadY, float Damage)
{
	FHitResult DebugHit;
	FHitResult Hit;

	FVector TraceStart = PlayerCameraManager->GetCameraLocation() - PlayerCharacter->GetActorUpVector() * 10;
	FVector TraceEnd = PlayerCameraManager->GetCameraLocation() + PlayerCameraManager->GetActorForwardVector() * 10000.0f + PlayerCameraManager->GetActorRightVector() * SpreadX + PlayerCameraManager->GetActorUpVector() * SpreadY;
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, DebugHit.bBlockingHit ? FColor::Blue : FColor::Red, false, 0.5f, 0, 1.0f);

	FCollisionQueryParams IgnorePlayer;
	IgnorePlayer.AddIgnoredActor(PlayerCharacter->GetUniqueID());

	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECollisionChannel::ECC_PhysicsBody, IgnorePlayer);

	AActor* ActorHit = Hit.GetActor();

	// If Hit has Health, Deal Damage
	if (ActorHit && IsValid(ActorHit->GetComponentByClass<UHealthComponent>()))
	{
		GEngine->AddOnScreenDebugMessage(0, 0.5f, FColor::White, "HIT DUMMY");

		ActorHit->TakeDamage(Damage, FDamageEvent(), this, PlayerCharacter);
	}
	//If Hit Is Simulating Physics, Add Impulse
	else if (ActorHit && ActorHit->GetComponentByClass<UStaticMeshComponent>()->IsSimulatingPhysics())
	{
		ActorHit->GetComponentByClass<UStaticMeshComponent>()->AddRadialImpulse(Hit.ImpactPoint, 128, 50 * Damage, ERadialImpulseFalloff(), true);
	}
}

void AShooterPlayerController::ChargeShot(float TimeToCharge)
{
	if (CurrentPistolCharge < TimeToCharge)
	{
		CurrentPistolCharge += CurrentDeltaTime;
	}
	else
	{
		CurrentPistolCharge = TimeToCharge;
	}
}

void AShooterPlayerController::FireChargedShot(float Charge)
{
	CurrentPistolCharge = 0.0f;

	float Damage = Charge / PistolChargeSpeed * PistolMaxDamage;

	ShootHitscan(0, 0, Damage);
}

void AShooterPlayerController::ShootProjectile(float Velocity)
{
	AActor* SpawnedGrenade;
	SpawnedGrenade = GetWorld()->SpawnActor(GrenadeProjectile);

	if (!SpawnedGrenade) return;

	SpawnedGrenade->SetActorLocation(PlayerCameraManager->GetCameraLocation() + PlayerCameraManager->GetActorForwardVector() * 150);
	SpawnedGrenade->GetComponentByClass<UStaticMeshComponent>()->SetSimulatePhysics(true);
	SpawnedGrenade->GetComponentByClass<UStaticMeshComponent>()->AddImpulse(PlayerCameraManager->GetActorForwardVector() * Velocity + PlayerCharacter->GetVelocity());
}



void AShooterPlayerController::Shoot()
{
	float RandomSpreadX;
	float RandomSpreadY;

	switch (EquippedWeapon)
	{
	case WeaponType::RIFLE:
		if (RifleAmmo > 0 && FireRate <= 0)
		{
			RandomSpreadX = FMath::RandRange(-RifleSpread, RifleSpread);
			RandomSpreadY = FMath::RandRange(-RifleSpread, RifleSpread);
			ShootHitscan(RandomSpreadX, RandomSpreadY, RifleDamage);
			RifleAmmo--;
			FireRate = RifleFireRate;
		}
		break;
	case WeaponType::SHOTGUN:
		if (ShotgunAmmo > 0)
		{
			for (int i = 0; i < ShotgunPellets; i++)
			{
				RandomSpreadX = FMath::RandRange(-ShotgunSpread, ShotgunSpread);
				RandomSpreadY = FMath::RandRange(-ShotgunSpread, ShotgunSpread);
				ShootHitscan(RandomSpreadX, RandomSpreadY, ShotgunPelletDamage);
			}
			ShotgunAmmo--;
		}
		break;
	case WeaponType::GRENADELAUNCHER:
		if (GrenadeLauncherAmmo > 0)
		{
			ShootProjectile(GrenadeVelocity);
			GrenadeLauncherAmmo--;
		}
		break;
	default:
		break;
	}
}

void AShooterPlayerController::EquipPistol()
{
	EquippedWeapon = WeaponType::PISTOL;
}

void AShooterPlayerController::EquipRifle()
{
	EquippedWeapon = WeaponType::RIFLE;
}

void AShooterPlayerController::EquipShotgun()
{
	EquippedWeapon = WeaponType::SHOTGUN;
}

void AShooterPlayerController::EquipGrenadeLauncher()
{
	EquippedWeapon = WeaponType::GRENADELAUNCHER;
}

void AShooterPlayerController::AddAmmo(int RifleAddition, int ShotgunAddition, int GrenadeLauncherAddition, PickupType Type)
{
	if (Type == PickupType::RIFLE_AMMO) RifleAmmo += RifleAddition;
	else if (Type == PickupType::SHOTGUN_AMMO) ShotgunAmmo += ShotgunAddition;
	else if (Type == PickupType::GRENADE_LAUNCHER_AMMO) GrenadeLauncherAmmo += GrenadeLauncherAddition;
}

void AShooterPlayerController::Tick(float DeltaTime)
{
	CurrentDeltaTime = DeltaTime;

	if (!PlayerCharacter->GetCharacterMovement()->IsFalling() && DashCooldown > 0.0f)
	{
		DashCooldown -= DeltaTime;

		//FString DashText = FString::SanitizeFloat(DashCooldown);
		//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, TEXT("Dash Cooldown = " + DashText));
	}

	if (FireRate > 0) FireRate -= DeltaTime;
}
