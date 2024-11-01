// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyCharacter.h"
#include "EnemyAIController.generated.h"



UCLASS()
class GERA_MOVEMENTSHOOTER_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

protected:
	
	UPROPERTY()
	AEnemyCharacter* EnemyCharacter;

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AEnemyCharacter* GetEnemyCharacter();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EEnemyType GetEnemyType() { return GetEnemyCharacter()->GetEnemyType(); }

	virtual void Tick(float DeltaTime) override;
};
