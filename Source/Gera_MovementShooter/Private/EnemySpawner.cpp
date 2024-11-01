// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"

#include "EnemyCharacter.h"
#include "HealthComponent.h"
#include "InventoryComponent.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}



// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	// SpawnEnemy();
	
}

// AEnemyCharacter* AEnemySpawner::SpawnEnemy()
// {
// 	AEnemyCharacter* SpawnedEnemy = GetWorld()->SpawnActor<AEnemyCharacter>(EnemyToSpawn.LoadSynchronous(), GetActorLocation(), GetActorRotation());
// 	
// 	SpawnedEnemy->GetInventoryComponent()->SwapWeapons(EWeaponSlot::Primary, EnemyWeapon);
// 	
// 	return SpawnedEnemy;
// }

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

