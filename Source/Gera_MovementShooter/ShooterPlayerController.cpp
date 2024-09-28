// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"

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

void AShooterPlayerController::ShootHitscan(float SpreadX, float SpreadY, float Damage, FVector ShotOrigin)
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
	
	// If Hit has Health, Deal Damage
	if (ActorHit && IsValid(ActorHit->GetComponentByClass<UHealthComponent>()))
	{
		GEngine->AddOnScreenDebugMessage(0, 0.5f, FColor::White, "HIT DUMMY");
	
		ActorHit->TakeDamage(Damage, FDamageEvent(), this, PlayerCharacter);
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

void AShooterPlayerController::ChargeShot(float MinCharge, float MaxCharge)
{
	// if (CurrentPistolCharge < TimeToCharge)
	// {
	// 	CurrentPistolCharge += CurrentDeltaTime;
	// }
	// else
	// {
	// 	CurrentPistolCharge = TimeToCharge;
	// }
}

void AShooterPlayerController::FireChargedShot(float Charge, FVector ShotOrigin)
{
	// CurrentPistolCharge = 0.0f;
	//
	// float Damage = Charge / PistolChargeSpeed * PistolMaxDamage;
	//
	// ShootHitscan(0, 0, Damage);
}

void AShooterPlayerController::ShootProjectile(float Velocity, FVector ShotOrigin)
{
	const TSubclassOf<AActor> ProjectileActor = PlayerCharacter->EquippedWeapon.GetRow<FWeaponData>("")->ProjectileActor;
	AActor* SpawnedProjectile = GetWorld()->SpawnActor(ProjectileActor);
	
	if (!SpawnedProjectile) return;
	
	SpawnedProjectile->SetActorLocation(PlayerCameraManager->GetCameraLocation() + PlayerCameraManager->GetActorForwardVector() * 150);
	SpawnedProjectile->SetActorRotation(PlayerCameraManager->GetCameraRotation());
	SpawnedProjectile->GetComponentByClass<UProjectileMovementComponent>()->Velocity = Velocity * PlayerCameraManager->GetCameraRotation().Vector();
}



void AShooterPlayerController::Shoot()
{
	FWeaponData* EquippedWeaponData = PlayerCharacter->EquippedWeapon.GetRow<FWeaponData>("");
	EProjectileType EquippedProjectileType = EquippedWeaponData->ProjectileType;
	float EquippedDamage = EquippedWeaponData->Damage;
	float EquippedSpread = EquippedWeaponData->MaxSpread;
	float ProjectileVelocity = EquippedWeaponData->ProjectileVelocity;
	float MinCharge = EquippedWeaponData->MinChargeTime;
	float MaxCharge = EquippedWeaponData->MaxChargeTime;

	FVector Origin = FVector::ZeroVector;
	if(PlayerCharacter->WeaponChildComponent->GetChildActor())
	{
		Origin = PlayerCharacter->WeaponChildComponent->GetChildActor()->GetComponentByClass<UArrowComponent>()->GetComponentTransform().GetLocation();
	}
	
	float RandomSpreadX = FMath::RandRange(-EquippedSpread, EquippedSpread);
	float RandomSpreadY = FMath::RandRange(-EquippedSpread, EquippedSpread);

	switch (EquippedProjectileType)
	{
	case EProjectileType::Hitscan:
		ShootHitscan(RandomSpreadX, RandomSpreadY, EquippedDamage, Origin);
		break;
	case EProjectileType::Projectile:
		ShootProjectile(ProjectileVelocity, Origin);
		break;
	case EProjectileType::ChargedHitscan:
		ChargeShot(MinCharge, MaxCharge);
		break;
	case EProjectileType::ChargedProjectile:
		ChargeShot(MinCharge, MaxCharge);
		break;
	default:
		break;
	}
	
}

// void AShooterPlayerController::EquipWeapon(EWeaponType NewWeaponType)
// {
// 	// EquippedWeapon = NewWeaponType;
//
// 	// switch (NewWeaponType)
// 	// {
// 	// case EWeaponType::KNIFE:
// 	// 	if (IsValid(KnifeActor))
// 	// 	{
// 	// 		PlayerCharacter->WeaponChildComponent->SetChildActorClass(KnifeActor);
// 	// 		WeaponMuzzleArrowComponent = nullptr;
// 	// 	}
// 	// 	break;
// 	// case EWeaponType::PISTOL:
// 	// 	if (IsValid(PistolActor))
// 	// 	{
// 	// 		PlayerCharacter->WeaponChildComponent->SetChildActorClass(PistolActor);
// 	// 	}
// 	// 	else WeaponMuzzleArrowComponent = nullptr;
// 	// 	break;
// 	// case EWeaponType::RIFLE:
// 	// 	if (IsValid(RifleActor))
// 	// 	{
// 	// 		PlayerCharacter->WeaponChildComponent->SetChildActorClass(RifleActor);
// 	// 	}
// 	// 	else WeaponMuzzleArrowComponent = nullptr;
// 	// 	break;
// 	// case EWeaponType::SHOTGUN:
// 	// 	if (IsValid(ShotgunActor))
// 	// 	{
// 	// 		PlayerCharacter->WeaponChildComponent->SetChildActorClass(ShotgunActor);
// 	// 	}
// 	// 	else WeaponMuzzleArrowComponent = nullptr;
// 	// 	break;
// 	// case EWeaponType::GRENADELAUNCHER:
// 	// 	if (IsValid(GrenadeLauncherActor))
// 	// 	{
// 	// 		PlayerCharacter->WeaponChildComponent->SetChildActorClass(GrenadeLauncherActor);
// 	// 	}
// 	// 	else WeaponMuzzleArrowComponent = nullptr;
// 	// 	break;
// 	// default:
// 	// 	break;
// 	// }
// 	//
// 	// if (NewWeaponType != EWeaponType::KNIFE && PlayerCharacter->WeaponChildComponent)
// 	// {
// 	// 	WeaponMuzzleArrowComponent = PlayerCharacter->WeaponChildComponent->GetChildActor()->GetComponentByClass<UArrowComponent>();
// 	// }
// }

void AShooterPlayerController::Tick(float DeltaTime)
{
	CurrentDeltaTime = DeltaTime;

	if (!PlayerCharacter->GetCharacterMovement()->IsFalling() && DashCooldown > 0.0f)
	{
		DashCooldown -= DeltaTime;

		//FString DashText = FString::SanitizeFloat(DashCooldown);
		//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, TEXT("Dash Cooldown = " + DashText));
	}

	// if (FireRate > 0) FireRate -= DeltaTime;
}
