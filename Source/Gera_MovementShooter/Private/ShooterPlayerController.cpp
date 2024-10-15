// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"

#include "ItemData.h"
#include "ThrowableActor.h"
#include "VectorTypes.h"
#include "GameFramework/ProjectileMovementComponent.h"


void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = GetPawn<AShooterPlayerCharacter>();

	if (PlayerCharacter)
	{
		PlayerInventoryComp = PlayerCharacter->GetComponentByClass<UInventoryComponent>();
		PlayerHealthComp = PlayerCharacter->GetComponentByClass<UHealthComponent>();		
	}


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

	if (DashCooldownValue <= 0.0f && !PlayerCharacter->bIsCrouched)
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
		DashVelocity = PlayerCharacter->GetMovementComponent()->GetMaxSpeed() * 10;
		PlayerCharacter->LaunchCharacter(DashDirection * DashVelocity, false, false);
		DashCooldownValue = DashCooldownLength;

		GetWorldTimerManager().SetTimer(DashTimer, this, &AShooterPlayerController::StopDash, DashDuration, false);
	}
	
}

void AShooterPlayerController::StopDash()
{
	if (!IsValid(PlayerCharacter)) return;

	FVector CurrentVelocity = PlayerCharacter->GetVelocity();
	FVector VelocityDirection;
	float CurrentSpeed;

	CurrentVelocity.ToDirectionAndLength(VelocityDirection, CurrentSpeed);
	
	float MaxMoveSpeed = PlayerCharacter->GetMovementComponent()->GetMaxSpeed();
	
	if (CurrentSpeed > MaxMoveSpeed)
	{
		PlayerCharacter->GetMovementComponent()->Velocity = VelocityDirection * MaxMoveSpeed;
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


void AShooterPlayerController::ShootHitscan(float SpreadX, float SpreadY, FVector ShotOrigin, float Damage)
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
		GEngine->AddOnScreenDebugMessage(0, 0.5f, FColor::White, "HIT ACTOR");
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


void AShooterPlayerController::ChargeShot(float MaxCharge)
{
	if (PlayerCharacter->InventoryComponent->GetAmmo(EAmmoType::EnergyAmmo) <= 0)
	{
		ResetWeapon();
		return;
	}
	
	if (CurrentWeaponCharge < MaxCharge)
	{
		CurrentWeaponCharge += CurrentDeltaTime;
	}
	else
	{
		CurrentWeaponCharge = MaxCharge;
	}
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
	float MaxCharge = EquippedWeaponData.MaxChargeTime;

	FVector Origin = FVector::ZeroVector;
	if(PlayerCharacter->WeaponChildComponent->GetChildActor())
	{
		Origin = PlayerCharacter->WeaponChildComponent->GetChildActor()->GetComponentByClass<UArrowComponent>()->GetComponentTransform().GetLocation();
	}
	
	if (PlayerCharacter->InventoryComponent->GetAmmo(EquippedWeaponData.AmmoType) <= 0) return;
	
	float RandomSpreadX = FMath::RandRange(-EquippedSpread, EquippedSpread);
	float RandomSpreadY = FMath::RandRange(-EquippedSpread, EquippedSpread);

	if (FireRate > 0 || (!SemiAutoCanFire && !EquippedWeaponData.CanCharge)) return;
	
	switch (EquippedProjectileType)
	{
	case EProjectileType::Hitscan:
		if (EquippedWeaponData.IsShotgun)
		{
			for (int i = 0; i < EquippedWeaponData.PelletCount; i++)
			{
				RandomSpreadX = FMath::RandRange(-EquippedSpread, EquippedSpread);
				RandomSpreadY = FMath::RandRange(-EquippedSpread, EquippedSpread);
				ShootHitscan(RandomSpreadX, RandomSpreadY, Origin, EquippedDamage);
			}
		}
		else if (EquippedWeaponData.CanCharge)
		{
			ShootHitscan(RandomSpreadX, RandomSpreadY, Origin, EquippedDamage * CurrentWeaponCharge / MaxCharge);
		}
		else
		{
			ShootHitscan(RandomSpreadX, RandomSpreadY, Origin, EquippedDamage);			
		}

		if (EquippedWeaponData.CanCharge)
		{
			PlayerCharacter->InventoryComponent->RemoveAmmo(EquippedWeaponData.AmmoType, FMath::RoundToInt(EquippedWeaponData.AmmoCost * CurrentWeaponCharge / MaxCharge));			
		}
		else
		{
			PlayerCharacter->InventoryComponent->RemoveAmmo(EquippedWeaponData.AmmoType, EquippedWeaponData.AmmoCost);			
		}

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

void AShooterPlayerController::UseThrowableItem()
{
	FItemData* ThrowableData = PlayerInventoryComp->EquipmentSlot.GetRow<FItemData>("");

	int EquipmentCount = PlayerInventoryComp->GetEquipment(ThrowableData->Name);

	if (EquipmentCount <= 0) return;
	
	const TSubclassOf<AActor> ThrowableActor = ThrowableData->ItemActor;
	AThrowableActor* SpawnedThrowable = Cast<AThrowableActor>(GetWorld()->SpawnActor(ThrowableActor));
	//AActor* SpawnedThrowable = GetWorld()->SpawnActor(ThrowableActor);
	
	if (!SpawnedThrowable) return;
	
	SpawnedThrowable->SetActorLocation(PlayerCameraManager->GetTransform().GetLocation() + PlayerCameraManager->GetActorForwardVector() * 150);
	SpawnedThrowable->GetComponentByClass<UStaticMeshComponent>()->SetSimulatePhysics(true);
	SpawnedThrowable->GetComponentByClass<UStaticMeshComponent>()->AddImpulse(ThrowableVelocity * PlayerCameraManager->GetCameraRotation().Vector(), "None", true);
	SpawnedThrowable->SetDamage(ThrowableData->Damage);
	SpawnedThrowable->SetBlastRadius(ThrowableData->BlastRadius);

	if (ThrowableData->BlastRadius > 0) SpawnedThrowable->StartFuseTimer(ThrowableData->FuseTime);
	
	PlayerInventoryComp->RemoveEquipment(ThrowableData->Name, 1);
}

void AShooterPlayerController::UseBuffItem()
{
	EBuffEffects CurrentBuffEffect = PlayerInventoryComp->GetEquippedSupportData().BuffEffect;
	switch (CurrentBuffEffect)
	{
	case EBuffEffects::None:
		break;
	case EBuffEffects::Heal:
		PlayerHealthComp->AddHealth(PlayerInventoryComp->GetEquippedSupportData().HealAmount);
		break;
	case EBuffEffects::SpeedBoost:
		break;
	case EBuffEffects::DamageBoost:
		break;
	case EBuffEffects::SlowTime:
		break;
	default: ;
	}
}


void AShooterPlayerController::Tick(float DeltaTime)
{
	CurrentDeltaTime = DeltaTime;

	if (!PlayerCharacter->GetCharacterMovement()->IsFalling() && DashCooldownValue > 0.0f)
	{
		DashCooldownValue -= DeltaTime;

		//FString DashText = FString::SanitizeFloat(DashCooldownValue);
		//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, TEXT("Dash Cooldown = " + DashText));
	}

	if (FireRate > 0) FireRate -= DeltaTime * GetEquippedWeaponData().FireRate * (GetEquippedWeaponData().FireRate / 60);
}
