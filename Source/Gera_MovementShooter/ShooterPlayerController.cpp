// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"

void AShooterPlayerController::TimerFunction()
{

}

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

	if (DashCooldown <= 0.0f)
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

void AShooterPlayerController::Shoot()
{
	if (Ammo > 0 && FireRate <= 0)
	{
		FHitResult Hit;

		FVector TraceStart = PlayerCameraManager->GetCameraLocation() - PlayerCharacter->GetActorUpVector() * 10;
		FVector TraceEnd = PlayerCameraManager->GetCameraLocation() + PlayerCameraManager->GetActorForwardVector() * 10000.0f;
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, Hit.bBlockingHit ? FColor::Blue : FColor::Red, false, 0.5f, 0, 1.0f);

		Ammo--;
		FireRate = DefaultFireRate;
	}
}

void AShooterPlayerController::Tick(float DeltaTime)
{
	if (!PlayerCharacter->GetCharacterMovement()->IsFalling() && DashCooldown > 0.0f)
	{
		DashCooldown -= DeltaTime;

		//FString DashText = FString::SanitizeFloat(DashCooldown);
		//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, TEXT("Dash Cooldown = " + DashText));
	}

	if (FireRate > 0) FireRate -= DeltaTime;
}
