// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "ExplosiveComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"


void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = GetPawn<AShooterPlayerCharacter>();

	DefaultFOV = GetPlayerCharacter()->GetFirstPersonCameraComponent()->FieldOfView;

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


void AShooterPlayerController::AddLookInput(const FVector2D Input)
{
	AddYawInput(Input.X);
	AddPitchInput(Input.Y);
	GEngine->AddOnScreenDebugMessage(4, 0.2f, FColor::Red, "Look Updated: " + Input.ToString());
}

void AShooterPlayerController::Jump()
{
	if (!IsValid(PlayerCharacter)) return;

	PlayerCharacter->Jump();
}


void AShooterPlayerController::Dash()
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


void AShooterPlayerController::ShootHitscan(float WeaponSpreadInDegrees, const FVector ShotOrigin, const FVector ShotDirection, float Damage)
{
	FHitResult Hit;

	if (IsDamageBoostActive)
	{
		Damage *= DamageBoostMultiplier;
	}

	const FVector CameraLocation = GetPlayerCharacter()->GetFirstPersonCameraComponent()->GetComponentLocation();
	const FVector CameraDirection = GetPlayerCharacter()->GetFirstPersonCameraComponent()->GetForwardVector();
	
	const FVector TraceStart = ShotOrigin;
	const FVector TraceEnd = CameraLocation + UKismetMathLibrary::RandomUnitVectorInConeInDegrees(CameraDirection, WeaponSpreadInDegrees / 2) * 50000;

	FCollisionQueryParams IgnorePlayer;
	IgnorePlayer.AddIgnoredActor(PlayerCharacter->GetUniqueID());

	
	
	//Initial Trace to get Target
	GetWorld()->LineTraceSingleByChannel(Hit, CameraLocation, TraceEnd, ECC_PhysicsBody, IgnorePlayer);
	// The Actual Shot Fired.
	DrawDebugLine(GetWorld(), TraceStart, Hit.bBlockingHit ? Hit.ImpactPoint : Hit.TraceEnd, FColor::Red, false, 0.2f, 0, 0.5f);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, Hit.bBlockingHit ? Hit.ImpactPoint : Hit.TraceEnd, ECC_PhysicsBody, IgnorePlayer);
	
	
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


void AShooterPlayerController::ShootProjectile(float WeaponSpreadInDegrees, float Velocity, FVector ShotOrigin, float Damage)
{
	const TSubclassOf<AActor> ProjectileActor = PlayerCharacter->EquippedWeapon.GetRow<FWeaponData>("")->ProjectileActor;
	AActor* SpawnedProjectile = GetWorld()->SpawnActor(ProjectileActor);

	if (!SpawnedProjectile) return;
	
	SpawnedProjectile->SetActorLocation(ShotOrigin + PlayerCameraManager->GetActorForwardVector() * 150);
	SpawnedProjectile->GetComponentByClass<UProjectileMovementComponent>()->Velocity = Velocity * PlayerCameraManager->GetCameraRotation().Vector();
}


void AShooterPlayerController::Shoot(bool InfiniteAmmo)
{
	FWeaponData const EquippedWeaponData = GetEquippedWeaponData();
	EProjectileType const EquippedProjectileType = EquippedWeaponData.ProjectileType;
	float const EquippedDamage = EquippedWeaponData.Damage;
	float const ProjectileVelocity = EquippedWeaponData.ProjectileVelocity;
	float const MaxCharge = EquippedWeaponData.MaxChargeTime;

	FVector Origin = GetPlayerCharacter()->GetFirstPersonCameraComponent()->GetComponentLocation();
	Origin += GetPlayerCharacter()->GetFirstPersonCameraComponent()->GetForwardVector() * 10;
	if (PlayerCharacter->WeaponChildComponent->GetChildActor())
	{
		Origin = PlayerCharacter->WeaponChildComponent->GetChildActor()->GetComponentByClass<UArrowComponent>()->GetComponentLocation();
	}

	if (InfiniteAmmo == false)
	{
		if (PlayerCharacter->InventoryComponent->GetAmmo(EquippedWeaponData.AmmoType) <= 0) return;
		if (FireRate > 0 || (!SemiAutoCanFire && !EquippedWeaponData.CanCharge)) return;
	}
	
	if (EquippedProjectileType == EProjectileType::Hitscan)
	{
		const FVector CameraForward = GetPlayerCharacter()->GetFirstPersonCameraComponent()->GetForwardVector();
		
		if (EquippedWeaponData.IsShotgun)
		{
			for (int i = 0; i < EquippedWeaponData.PelletCount; i++)
			{
				ShootHitscan(GetShotSpreadInDegrees(), Origin, CameraForward, EquippedDamage * FMath::Clamp(CurrentWeaponCharge / MaxCharge, 1.0f, MAX_FLT));
			}
		}
		else ShootHitscan(GetShotSpreadInDegrees(), Origin, CameraForward, EquippedDamage * FMath::Clamp(CurrentWeaponCharge / MaxCharge, 1.0f, MAX_FLT));
	}
	else if (EquippedProjectileType == EProjectileType::Projectile)
	{
		if (EquippedWeaponData.IsShotgun)
		{
			for (int i = 0; i < EquippedWeaponData.PelletCount; i++)
			{
				ShootProjectile(GetShotSpreadInDegrees(), ProjectileVelocity, Origin, EquippedDamage * FMath::Clamp(CurrentWeaponCharge / MaxCharge, 1.0f, MAX_FLT));
			}
		}
		else ShootProjectile(GetShotSpreadInDegrees(), ProjectileVelocity, Origin, EquippedDamage * FMath::Clamp(CurrentWeaponCharge / MaxCharge, 1.0f, MAX_FLT));
	}

	if (IsAiming())
	{
		AddRecoil(GetEquippedWeaponData().AimingRecoil);
	}
	else AddRecoil(GetEquippedWeaponData().HipfireRecoil);
	

	CurrentWeaponCharge = 0;

	if (EquippedWeaponData.WeaponType == EWeaponType::Automatic)
	{
		FireRate = EquippedWeaponData.FireRate;
	}
	else SemiAutoCanFire = false;
}

float AShooterPlayerController::GetShotSpreadInDegrees()
{
	if (float Spread = GetEquippedWeaponData().SpreadInDegrees)
	{
		if (IsAiming())
		{
			if (GetEquippedWeaponData().AimingSpreadOverride) Spread = GetEquippedWeaponData().AimingSpreadInDegrees;
			else Spread /= GetEquippedWeaponData().ZoomMultiplier;
		}
		
		return Spread;
	}
	return 0;
}

void AShooterPlayerController::Aim(float ZoomMultiplier)
{
	if (IsAiming() || ZoomMultiplier == 0) return;

	GetPlayerCharacter()->GetFirstPersonCameraComponent()->SetFieldOfView(DefaultFOV / ZoomMultiplier);
}

void AShooterPlayerController::StopAiming()
{
	GetPlayerCharacter()->GetFirstPersonCameraComponent()->SetFieldOfView(DefaultFOV);
}

bool AShooterPlayerController::IsAiming()
{
	if (GetPlayerCharacter()->GetFirstPersonCameraComponent()->FieldOfView != DefaultFOV)
	{
		return true;
	}
	return false;
}

void AShooterPlayerController::AddRecoil(FVector2D RecoilAmount)
{
	if (RecoilTarget == FVector2D::ZeroVector)
	{
		RecoilTarget = FVector2D(FMath::RandRange(-RecoilAmount.X, RecoilAmount.X), -RecoilAmount.Y);
	}
	else
	{
		RecoilTarget += FVector2D(FMath::RandRange(-RecoilAmount.X, RecoilAmount.X), -RecoilAmount.Y);
		RecoilProgress = FVector2D::ZeroVector;
	}

	if (RecoilProgress != RecoilTarget)
	{
		GetWorldTimerManager().ClearTimer(RecoilTimer);
		GetWorldTimerManager().SetTimer(RecoilTimer, this, &AShooterPlayerController::UpdateRecoil, 0.0001f, false);
		UpdateRecoil();
	}
	
	 //GEngine->AddOnScreenDebugMessage(4, 0.2f, FColor::Red, "Recoil Added: " + RecoilAmount.ToString());
	
}

void AShooterPlayerController::UpdateRecoil()
{
	FVector2D DeltaRecoilProgress = FVector2D::ZeroVector;
	
	if (RecoilTarget != FVector2D::ZeroVector && RecoilProgress != RecoilTarget)
	{
		const FVector2D PreviousRecoilProgress = RecoilProgress;
		RecoilProgress = FMath::Vector2DInterpTo(RecoilProgress, RecoilTarget, GetWorld()->DeltaTimeSeconds, 80.0f);
		DeltaRecoilProgress = RecoilProgress - PreviousRecoilProgress;
		
		AddLookInput(DeltaRecoilProgress);

		GetWorldTimerManager().SetTimer(RecoilTimer, this, &AShooterPlayerController::UpdateRecoil, 0.0001f, false);
	}
	else if (RecoilProgress == RecoilTarget)
	{
		RecoilTarget = FVector2D::ZeroVector;
		RecoilProgress = FVector2D::ZeroVector;
	}
	
	GEngine->AddOnScreenDebugMessage(1, 0.2f, FColor::Yellow, "RecoilProgress = " + RecoilProgress.ToString());
	GEngine->AddOnScreenDebugMessage(2, 0.2f, FColor::Blue, "DeltaRecoilProgress = " + DeltaRecoilProgress.ToString());
	GEngine->AddOnScreenDebugMessage(3, 0.2f, FColor::Green, "RecoilTarget = " + RecoilTarget.ToString());
}


void AShooterPlayerController::ResetWeapon()
{
	FireRate = 0;
	CurrentWeaponCharge = 0;
	SemiAutoCanFire = true;
	RecoilTarget = FVector2D::ZeroVector;
	RecoilProgress = FVector2D::ZeroVector;

	GEngine->AddOnScreenDebugMessage(4, 0.2f, FColor::Yellow, "MouseRecoilTracker = " + MouseRecoilTracker.ToString());
	GEngine->AddOnScreenDebugMessage(5, 0.2f, FColor::Yellow, "MouseRecoilReturnLimit = " + MouseRecoilReturnLimit.ToString());

	//AddLookInput(-RecoilTarget);
	
	// if (MouseRecoilTracker.ComponentwiseAllLessOrEqual(MouseRecoilReturnLimit))
	// {
	// 	
	// }
	
	MouseRecoilTracker = FVector2D::ZeroVector;
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
	// ------ DASHING ------
	
	if (!PlayerCharacter->GetCharacterMovement()->IsFalling() && DashCooldownValue > 0.0f)
	{
		DashCooldownValue -= DeltaTime;

		//FString DashText = FString::SanitizeFloat(DashCooldownValue);
		//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, TEXT("Dash Cooldown = " + DashText));
	}

	// ------ FIRE RATE ------

	if (FireRate > 0) FireRate -= DeltaTime * GetEquippedWeaponData().FireRate * (GetEquippedWeaponData().FireRate / 60);

}
