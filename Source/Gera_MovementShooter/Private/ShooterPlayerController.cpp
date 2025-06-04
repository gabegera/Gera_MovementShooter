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


void AShooterPlayerController::AddLookInput(FVector2D Input)
{
	if (IsAiming()) Input *= MouseSensitivity / GetEquippedWeaponData().ZoomMultiplier;
	else Input *= MouseSensitivity;

	if (RecoilCache != FVector2D::ZeroVector)
	{
		RecoilCache.Y -= FMath::Abs(Input.Y);
		if (RecoilCache.Y < 0) RecoilCache.Y = 0;
	}
	
	if (GetWorldTimerManager().IsTimerActive(RecoilTimer) == false) RecoilCache = FVector2D::ZeroVector;
	
	AddYawInput(Input.X);
	AddPitchInput(Input.Y);
}

void AShooterPlayerController::Jump()
{
	if (!IsValid(PlayerCharacter)) return;

	PlayerCharacter->Jump();
}


void AShooterPlayerController::Dash()
{
	DashCooldownValue = DashCooldownLength;
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

void AShooterPlayerController::Shoot(bool InfiniteAmmo)
{
	float DamageAfterMultiplier = GetDamage();
	if (IsDamageBoostActive) DamageAfterMultiplier *= DamageBoostMultiplier;

	if (!InfiniteAmmo)
	{
		if (GetInventory()->GetAmmo(GetAmmoType()) <= 0) return;
		if (FireRate > 0 || (!SemiAutoCanFire && !GetEquippedWeaponData().CanCharge)) return;
		GetInventory()->RemoveAmmo(GetEquippedWeaponData().AmmoType, GetEquippedWeaponData().AmmoCost);
	}

	//Shot Starts at Player Camera with an offset of 10.0f
	FVector ShotStart = GetFPCameraLocation() + GetFPCameraForward() * 10.0f + GetFPCameraUp() * 10.0f;
	FVector ShotTarget = GetFPCameraLocation() + GetFPCameraForward() * 50000.0f;

	// If Player has a weapon
	if (GetPlayerCharacter()->WeaponChildComponent->GetChildActor())
	{
		ShotStart = GetMuzzleLocation();

		FHitResult Hit;
		FCollisionQueryParams IgnorePlayer;
		IgnorePlayer.AddIgnoredActor(GetPlayerCharacter()->GetUniqueID());
		GetWorld()->LineTraceSingleByChannel(Hit, GetFPCameraLocation(), GetFPCameraLocation() + GetFPCameraForward() * 50000.0f, ECC_PhysicsBody, IgnorePlayer);
		
		ShotTarget = Hit.bBlockingHit ? Hit.ImpactPoint : Hit.TraceEnd;
	}
	
	for (int i = 0; i <= GetPelletCount(); i++)
	{
		if (GetProjectileType() == EProjectileType::Hitscan) {
			GetPlayerCharacter()->ShootHitscan(GetShotSpreadInDegrees(), ShotStart, ShotTarget, DamageAfterMultiplier);
		}
		else if (GetProjectileType() == EProjectileType::Projectile) {
        	GetPlayerCharacter()->ShootProjectile(GetProjectileActor(), GetShotSpreadInDegrees(), ShotStart, ShotTarget, GetProjectileVelocity(), DamageAfterMultiplier);
        }
		
		if (IsWeaponShotgun() == false) break;
	}
	
	if (IsAiming())
	{
		AddRecoil(GetAimingRecoil());
	}
	else AddRecoil(GetHipfireRecoil());
	
	if (GetWeaponType() == EWeaponType::Automatic)
	{
		FireRate = GetFireRate();
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
		RecoilTarget = RecoilAmount;;
	}
	else
	{
		RecoilTarget += RecoilAmount;
		RecoilProgress = FVector2D::ZeroVector;
		RecoilCache = FVector2D::ZeroVector;
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
		RecoilProgress = FMath::Vector2DInterpTo(RecoilProgress, RecoilTarget, GetWorld()->DeltaTimeSeconds, RecoilInterpSpeed);
		DeltaRecoilProgress = RecoilProgress - PreviousRecoilProgress;
		
		AddYawInput(DeltaRecoilProgress.X);
		AddPitchInput(DeltaRecoilProgress.Y);
		RecoilCache += DeltaRecoilProgress;

		GetWorldTimerManager().SetTimer(RecoilTimer, this, &AShooterPlayerController::UpdateRecoil, 0.0001f, false);
	}
	else if (RecoilProgress == RecoilTarget)
	{
		RecoilTarget = FVector2D::ZeroVector;
		RecoilProgress = FVector2D::ZeroVector;
	}
	
	// GEngine->AddOnScreenDebugMessage(1, 0.2f, FColor::Yellow, "RecoilProgress = " + RecoilProgress.ToString());
	// GEngine->AddOnScreenDebugMessage(2, 0.2f, FColor::Blue, "DeltaRecoilProgress = " + DeltaRecoilProgress.ToString());
	// GEngine->AddOnScreenDebugMessage(3, 0.2f, FColor::Green, "RecoilTarget = " + RecoilTarget.ToString());
}


void AShooterPlayerController::ResetWeapon()
{
	FireRate = 0;
	CurrentWeaponCharge = 0;
	SemiAutoCanFire = true;
	RecoilTarget = FVector2D::ZeroVector;
	RecoilProgress = FVector2D::ZeroVector;

	// GEngine->AddOnScreenDebugMessage(4, 0.2f, FColor::Yellow, "MouseRecoilTracker = " + MouseRecoilTracker.ToString());
	// GEngine->AddOnScreenDebugMessage(5, 0.2f, FColor::Yellow, "MouseRecoilReturnLimit = " + MouseRecoilReturnLimit.ToString());

	//AddLookInput(-RecoilTarget);
	
	// if (MouseRecoilTracker.ComponentwiseAllLessOrEqual(MouseRecoilReturnLimit))
	// {
	// 	
	// }
	
	// MouseRecoilTracker = FVector2D::ZeroVector;
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

FVector2D AShooterPlayerController::GetHipfireRecoil()
{
	const float RecoilX = FMath::RandRange(GetEquippedWeaponData().HipfireRecoilMinX, GetEquippedWeaponData().HipfireRecoilMaxX);
	const float RecoilY = GetEquippedWeaponData().HipfireRecoilY;

	return FVector2D(RecoilX, RecoilY);
}

FVector2D AShooterPlayerController::GetAimingRecoil()
{
	const float RecoilX = FMath::RandRange(GetEquippedWeaponData().AimingRecoilMinX, GetEquippedWeaponData().AimingRecoilMaxX);
	const float RecoilY = GetEquippedWeaponData().AimingRecoilY;

	return FVector2D(RecoilX, RecoilY);
}


void AShooterPlayerController::Tick(float DeltaTime)
{
	if (IsDamageBoostActive) GEngine->AddOnScreenDebugMessage(10, 0.2f, FColor::Red, TEXT("Damage Boost Active"));

	GEngine->AddOnScreenDebugMessage(4, 0.2f, FColor::Red, "Recoil Cache: " + RecoilCache.ToString());
	
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
