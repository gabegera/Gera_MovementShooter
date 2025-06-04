// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AISenseConfig_Sight.h"

ABaseEnemyAIController::ABaseEnemyAIController()
	:Super()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AI Perception");

	// Create the senses
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(FName("Sight Config"));

	// Configuring the sight sense
	SightConfig->SightRadius = 600.0f;
	SightConfig->LoseSightRadius = 700.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	// Assigning the sight and hearing senses to the AI Perception component
	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
}


void ABaseEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	// Register the OnTargetPerceptionUpdated function to the OnTargetPerceptionUpdated event
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseEnemyAIController::OnTargetPerceptionUpdated);
	
	// switch (GetEnemyType())
	// {
	// case EEnemyType::Melee:
	// 	RunBehaviorTree(MeleeBehaviorTree);
	// 	break;
	// case EEnemyType::Ranged:
	// 	RunBehaviorTree(RangedBehaviorTree);
	// 	break;
	// }
	//
	// GetBlackboardComponent()->SetValueAsObject(FName("Target"), GetPawn());
	// GetBlackboardComponent()->SetValueAsVector(FName("TargetLocation"), GetPawn()->GetActorLocation());
}

void ABaseEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	// // Check if the actor is valid
	// AActor* SensedActor = Actor;
	// if (SensedActor == nullptr) return;
	//
	// // Check if the stimulus is sight
	// if (Stimulus.Type == SightConfig->GetSenseID())
	// {
	// 	if (Stimulus.WasSuccessfullySensed())
	// 	{
	// 		// Check if the sensed actor is the player
	// 		if (Cast<AShooterPlayerCharacter>(SensedActor))
	// 		{
	// 			GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, TEXT("SAW PLAYER"));
	// 			//bCanSeePlayer = true;
	// 		}
	// 	}
	// 	else
	// 	{
	// 		// Check if the sensed actor is the player
	// 		if (Cast<AShooterPlayerCharacter>(SensedActor))
	// 		{
	// 			GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Green, TEXT("CAN'T SEE PLAYER"));
	// 			//bCanSeePlayer = false;
	// 		}
	// 	}
	// }
}

ABaseEnemyCharacter* ABaseEnemyAIController::GetEnemyCharacter()
{
	if (!EnemyCharacter)
	{
		EnemyCharacter = Cast<ABaseEnemyCharacter>(GetCharacter());
	}

	return EnemyCharacter;

}

void ABaseEnemyAIController::Shoot(FVector TargetLocation)
{
	FVector ShotStart = GetEnemyCharacter()->GetActorLocation();
	FVector ShotTarget = TargetLocation;
	
	if (GetEnemyCharacter()->WeaponChildComponent->GetChildActor())
	{
		ShotStart = GetMuzzleLocation();
	}

	FVector ShotDirection = UKismetMathLibrary::GetDirectionUnitVector(ShotStart, ShotTarget);
	for (int i = 0; i <= GetPelletCount(); i++)
	{
		if (i == 0) i++;
		
		if (GetProjectileType() == EProjectileType::Hitscan) {
			GetEnemyCharacter()->ShootHitscan(GetShotSpreadInDegrees(), ShotStart, ShotTarget, GetDamage());
		}
		else if (GetProjectileType() == EProjectileType::Projectile) {
			GetEnemyCharacter()->ShootProjectile(GetProjectileActor(), GetShotSpreadInDegrees(), ShotStart, ShotTarget, GetProjectileVelocity(), GetDamage());
		}
		
		if (IsWeaponShotgun() == false) break;
	}
	
	// if (GetCanShoot() == false) return;
	//
	// const TSubclassOf<AActor> ProjectileActor = GetEnemyCharacter()->EquippedWeapon.GetRow<FWeaponData>("")->ProjectileActor;
	// AActor* SpawnedProjectile = GetWorld()->SpawnActor(ProjectileActor);
	//
	// float FireRate = GetEnemyCharacter()->GetEquippedWeaponData().FireRate * (GetEnemyCharacter()->GetEquippedWeaponData().FireRate / 60);
	// if (FireRate == 0) FireRate = 0.5f;
	//
	// GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ABaseEnemyAIController::CanShootEnable, FireRate, false);
	//
	// if (!SpawnedProjectile) return;
	//
	// const FVector Origin = GetEnemyCharacter()->WeaponChildComponent->GetChildActor()->GetComponentByClass<UArrowComponent>()->GetComponentLocation();
	//
	// SpawnedProjectile->SetActorLocation(Origin);
	// SpawnedProjectile->GetComponentByClass<UProjectileMovementComponent>()->Velocity = Velocity * GetEnemyCharacter()->GetActorRotation().Vector();
	//
	// CanShootDisable();
}

float ABaseEnemyAIController::GetShotSpreadInDegrees()
{
	return GetEquippedWeaponData().SpreadInDegrees;
}

FWeaponData ABaseEnemyAIController::GetEquippedWeaponData()
{
	return GetEnemyCharacter()->GetEquippedWeaponData();
}

void ABaseEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// GetBlackboardComponent()->SetValueAsVector(FName("TargetLocation"), GetPawn()->GetActorLocation());
}
