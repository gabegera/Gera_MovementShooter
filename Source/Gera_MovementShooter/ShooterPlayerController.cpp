// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"

#include "IntVectorTypes.h"
#include "GameFramework/ProjectileMovementComponent.h"


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
		else
		{
			DashDirection = PlayerCharacter->GetActorRightVector() * InputX + PlayerCharacter->GetActorForwardVector() * InputY;
		}
		
		
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


void AShooterPlayerController::ShootHitscan(float SpreadX, float SpreadY, FVector ShotOrigin)
{	
	FHitResult DebugHit;
	FHitResult Hit;
	
	FVector TraceStart;
	if (ShotOrigin != FVector::ZeroVector)
	{
		TraceStart = ShotOrigin;
	}
	else
	{
		TraceStart = PlayerCameraManager->GetCameraLocation() - PlayerCharacter->GetActorUpVector() * 10;
	}
	
	FVector TraceEnd = PlayerCameraManager->GetCameraLocation() + PlayerCameraManager->GetActorForwardVector() * 10000.0f + PlayerCameraManager->GetActorRightVector() * SpreadX + PlayerCameraManager->GetActorUpVector() * SpreadY;
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, DebugHit.bBlockingHit ? FColor::Blue : FColor::Red, false, 0.5f, 0, 1.0f);
	
	FCollisionQueryParams IgnorePlayer;
	IgnorePlayer.AddIgnoredActor(PlayerCharacter->GetUniqueID());
	
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECollisionChannel::ECC_PhysicsBody, IgnorePlayer);
	
	AActor* ActorHit = Hit.GetActor();

	float Damage = GetEquippedWeaponData().Damage;
	if (GetEquippedWeaponData().CanCharge == true) Damage *= CurrentWeaponCharge;
	
	// If Hit has Health, Deal Damage
	if (ActorHit && IsValid(ActorHit->GetComponentByClass<UHealthComponent>()))
	{
		GEngine->AddOnScreenDebugMessage(0, 0.5f, FColor::White, "HIT ACTOR");
		
		ActorHit->GetComponentByClass<UHealthComponent>()->TakeDamage(Damage);
	}
	//If Hit Is Simulating Physics, Add Impulse
	else if (ActorHit && IsValid(ActorHit->GetComponentByClass<UStaticMeshComponent>()))
	{
		if (ActorHit->GetComponentByClass<UStaticMeshComponent>()->IsSimulatingPhysics())
		{
			ActorHit->GetComponentByClass<UStaticMeshComponent>()->AddRadialImpulse(Hit.ImpactPoint, 128, 50 * Damage, ERadialImpulseFalloff(), true);
		}
	}
}


void AShooterPlayerController::ChargeShot(float MaxCharge)
{
	if (CurrentWeaponCharge < MaxCharge)
	{
		CurrentWeaponCharge += CurrentDeltaTime;
	}
	else
	{
		CurrentWeaponCharge = MaxCharge;
	}
}


void AShooterPlayerController::FireChargedShot(float Charge, float MinCharge, FVector ShotOrigin)
{
	// CurrentPistolCharge = 0.0f;
	//
	// if (Charge < MinCharge) return;
	//
	// ShootHitscan(0, 0, ShotOrigin);
}


void AShooterPlayerController::ShootProjectile(float SpreadX, float SpreadY, float Velocity, FVector ShotOrigin)
{
	const TSubclassOf<AActor> ProjectileActor = PlayerCharacter->EquippedWeapon.GetRow<FWeaponData>("")->ProjectileActor;
	AActor* SpawnedProjectile = GetWorld()->SpawnActor(ProjectileActor);
	
	if (!SpawnedProjectile) return;
	
	SpawnedProjectile->SetActorLocation(ShotOrigin + PlayerCameraManager->GetActorForwardVector() * 150);
	SpawnedProjectile->GetComponentByClass<UProjectileMovementComponent>()->Velocity = Velocity * PlayerCameraManager->GetCameraRotation().Vector();
}


void AShooterPlayerController::Shoot()
{
	FWeaponData EquippedWeaponData = GetEquippedWeaponData();
	EProjectileType EquippedProjectileType = EquippedWeaponData.ProjectileType;
	float EquippedDamage = EquippedWeaponData.Damage;
	float EquippedSpread = EquippedWeaponData.MaxSpread;
	float ProjectileVelocity = EquippedWeaponData.ProjectileVelocity;
	float MinCharge = EquippedWeaponData.MinChargeTime;
	float MaxCharge = EquippedWeaponData.MaxChargeTime;

	FVector Origin = FVector::ZeroVector;
	if(PlayerCharacter->WeaponChildComponent->GetChildActor())
	{
		Origin = PlayerCharacter->WeaponChildComponent->GetChildActor()->GetComponentByClass<UArrowComponent>()->GetComponentTransform().GetLocation();
	}
	
	if (PlayerCharacter->InventoryComponent->GetAmmo(EquippedWeaponData.AmmoType) <= 0) return;
	
	float RandomSpreadX = FMath::RandRange(-EquippedSpread, EquippedSpread);
	float RandomSpreadY = FMath::RandRange(-EquippedSpread, EquippedSpread);

	if (FireRate > 0 || !SemiAutoCanFire) return;
	
	switch (EquippedProjectileType)
	{
	case EProjectileType::Hitscan:
		if (EquippedWeaponData.IsShotgun)
		{
			for (int i = 0; i < EquippedWeaponData.PelletCount; i++)
			{
				RandomSpreadX = FMath::RandRange(-EquippedSpread, EquippedSpread);
				RandomSpreadY = FMath::RandRange(-EquippedSpread, EquippedSpread);
				ShootHitscan(RandomSpreadX, RandomSpreadY, Origin);
			}
		}
		else
		{
			ShootHitscan(RandomSpreadX, RandomSpreadY, Origin);			
		}
		PlayerCharacter->InventoryComponent->RemoveAmmo(EquippedWeaponData.AmmoType, 1);
		break;
		
	case EProjectileType::Projectile:
		if (EquippedWeaponData.IsShotgun)
		{
			for (int i = 0; i < EquippedWeaponData.PelletCount; i++)
			{
				RandomSpreadX = FMath::RandRange(-EquippedSpread, EquippedSpread);
				RandomSpreadY = FMath::RandRange(-EquippedSpread, EquippedSpread);
				ShootProjectile(RandomSpreadX, RandomSpreadY, ProjectileVelocity, Origin);
			}
		}
		else
		{
			ShootProjectile(RandomSpreadX, RandomSpreadY, ProjectileVelocity, Origin);			
		}
		PlayerCharacter->InventoryComponent->RemoveAmmo(EquippedWeaponData.AmmoType, 1);
		break;
	
	default:
		break;
	}

	CurrentWeaponCharge = 0;

	if (EquippedWeaponData.WeaponType == EWeaponType::Automatic)
	{
		FireRate = EquippedWeaponData.FireRate;
	}
	else SemiAutoCanFire = false;
	
}


void AShooterPlayerController::ResetWeapon()
{
	FireRate = 0;
	CurrentWeaponCharge = 0;
	SemiAutoCanFire = true;
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

	if (FireRate > 0) FireRate -= DeltaTime * GetEquippedWeaponData().FireRate * (GetEquippedWeaponData().FireRate / 60);
}
