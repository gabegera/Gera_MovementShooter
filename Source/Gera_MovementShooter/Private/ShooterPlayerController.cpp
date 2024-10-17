// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"

#include "ExplosiveComponent.h"
#include "Serialization/BulkDataRegistry.h"


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

		// Add each mapping context, along with their priority values. Higher values out prioritize lower values.
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	FOnTimelineFloat ProgressUpdate;
	ProgressUpdate.BindUFunction(this, FName("UpdateDash"));

	DashTimeline.AddInterpFloat(DashCurve, ProgressUpdate);
}


void AShooterPlayerController::UseSpeedBoost()
{
	UCharacterMovementComponent* MovementComponent = PlayerCharacter->GetCharacterMovement();
	MovementComponent->MaxWalkSpeed *= SpeedBoostMultiplier;
	MovementComponent->MaxWalkSpeedCrouched *= SpeedBoostMultiplier;
}

void AShooterPlayerController::StopSpeedBoost()
{
	UCharacterMovementComponent* MovementComponent = PlayerCharacter->GetCharacterMovement();
	MovementComponent->MaxWalkSpeed /= SpeedBoostMultiplier;
	MovementComponent->MaxWalkSpeedCrouched /= SpeedBoostMultiplier;
}

void AShooterPlayerController::UseDamageBoost()
{
	IsDamageBoostActive = true;
}

void AShooterPlayerController::StopDamageBoost()
{
	IsDamageBoostActive = false;
}

void AShooterPlayerController::UseSlowTime()
{
	UGameplayStatics::SetGlobalTimeDilation(this, 1 / SlowTimeDivision);
}

void AShooterPlayerController::StopSlowTime() const
{
	UGameplayStatics::SetGlobalTimeDilation(this, 1);
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
	DashCooldownValue = DashCooldownLength;
	
	// DashDirection = InputX * PlayerCharacter->GetActorRightVector() + InputY * PlayerCharacter->GetActorForwardVector();
	// FHitResult FloorHit = PlayerCharacter->GetCharacterMovement()->CurrentFloor.HitResult;
	// DashDirection = GizmoMath::ProjectPointOntoPlane(DashDirection, FloorHit.Location, FloorHit.Normal);
	// DashDirection.Normalize();
	//
	// DashTimeline.PlayFromStart();
	
	// if (!IsValid(PlayerCharacter)) return;
	//
	// if (DashCooldownValue <= 0.0f && !PlayerCharacter->bIsCrouched)
	// {
	// 	FVector DashDirection;
	// 	if (InputX == 0 && InputY == 0)
	// 	{
	// 		DashDirection = PlayerCharacter->GetActorForwardVector();
	// 	}
	// 	else
	// 	{
	// 		DashDirection = PlayerCharacter->GetActorRightVector() * InputX + PlayerCharacter->GetActorForwardVector() * InputY;
	// 	}
	// 	DashVelocity = PlayerCharacter->GetMovementComponent()->GetMaxSpeed() * 10;
	// 	PlayerCharacter->LaunchCharacter(DashDirection * DashVelocity, false, false);
	//
	// 	GetWorldTimerManager().SetTimer(DashTimer, this, &AShooterPlayerController::StopDash, DashDuration, false);
	// }
}

void AShooterPlayerController::UpdateDash(float Alpha)
{
	// GEngine->AddOnScreenDebugMessage(10, 0.5f, FColor::Yellow, FString::SanitizeFloat(Alpha));
	//
	// PlayerCharacter->AddActorWorldOffset(DashDirection * DashVelocity * Alpha, true);
}

void AShooterPlayerController::Crouch()
{
	PlayerCharacter->Crouch();
}


void AShooterPlayerController::StopCrouch()
{
	PlayerCharacter->UnCrouch();
}

FWeaponData AShooterPlayerController::GetEquippedWeaponData()
{
	return *PlayerCharacter->EquippedWeapon.GetRow<FWeaponData>("");
}


void AShooterPlayerController::ShootHitscan(float SpreadX, float SpreadY, FVector ShotOrigin, float Damage)
{	
	FHitResult DebugHit;
	FHitResult Hit;

	if (IsDamageBoostActive)
	{
		Damage *= DamageBoostMultiplier;
	}
	
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

	UExplosiveComponent* ExplosiveComponent = SpawnedThrowable->GetComponentByClass<UExplosiveComponent>();
	if (ExplosiveComponent)
	{
		if (IsDamageBoostActive)
		{
			ExplosiveComponent->SetDamage(ThrowableData->Damage *= DamageBoostMultiplier);
		}
		else ExplosiveComponent->SetDamage(ThrowableData->Damage);
		ExplosiveComponent->SetBlastRadius(ThrowableData->BlastRadius);
		ExplosiveComponent->SetFuseTime(ThrowableData->FuseTime);
	}
	
	PlayerInventoryComp->RemoveEquipment(ThrowableData->Name, 1);
}

void AShooterPlayerController::UseBuffItem()
{
	if (PlayerInventoryComp->SupportItemSlot.IsNull()) return;
	
	float TimerLength = PlayerInventoryComp->GetEquippedSupportData().BuffDuration;
	EBuffEffects CurrentBuffEffect = PlayerInventoryComp->GetEquippedSupportData().BuffEffect;
	switch (CurrentBuffEffect)
	{
	case EBuffEffects::None:
		break;
	case EBuffEffects::Heal:
		PlayerHealthComp->AddHealth(PlayerInventoryComp->GetEquippedSupportData().HealAmount);
		break;
	case EBuffEffects::SpeedBoost:
		if (GetWorldTimerManager().GetTimerRemaining(BuffTimer) > 0) break;
		UseSpeedBoost();
		GetWorldTimerManager().SetTimer(BuffTimer, this, &AShooterPlayerController::StopSpeedBoost, TimerLength, false);
		break;
	case EBuffEffects::DamageBoost:
		if (GetWorldTimerManager().GetTimerRemaining(BuffTimer) > 0) break;
		UseDamageBoost();
		GetWorldTimerManager().SetTimer(BuffTimer, this, &AShooterPlayerController::StopDamageBoost, TimerLength, false);
		break;
	case EBuffEffects::SlowTime:
		if (GetWorldTimerManager().GetTimerRemaining(BuffTimer) > 0) break;
		UseSlowTime();
		GetWorldTimerManager().SetTimer(BuffTimer, this, &AShooterPlayerController::StopSlowTime, TimerLength / SlowTimeDivision, false);
		break;
	default:
		break;
	}

	FDataTableRowHandle Empty;
	PlayerInventoryComp->SupportItemSlot = Empty;
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
