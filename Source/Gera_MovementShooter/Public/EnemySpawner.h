// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemyCharacter.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class GERA_MOVEMENTSHOOTER_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<ABaseEnemyCharacter> EnemyToSpawn;

	UPROPERTY(EditAnywhere)
	FDataTableRowHandle EnemyWeapon;

	UPROPERTY(EditAnywhere)
	float EnemyHealth;

public:

	// UFUNCTION(BlueprintCallable)
	// ABaseEnemyCharacter* SpawnEnemy();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
