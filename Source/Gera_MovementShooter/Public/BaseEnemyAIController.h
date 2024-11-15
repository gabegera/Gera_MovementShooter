// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseEnemyCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "BaseEnemyAIController.generated.h"



UCLASS()
class GERA_MOVEMENTSHOOTER_API ABaseEnemyAIController : public AAIController
{
	GENERATED_BODY()

protected:

	ABaseEnemyAIController();

	UPROPERTY(VisibleDefaultsOnly, Category = "AI")
	class UAISenseConfig_Sight* SightConfig;
	
	UPROPERTY()
	ABaseEnemyCharacter* EnemyCharacter;
	
	// UPROPERTY(EditAnywhere, BlueprintReadOnly)
	// UAIPerceptionComponent* AIPerceptionComponent = nullptr;

	UPROPERTY()
	FTimerHandle FireRateTimerHandle;

	UPROPERTY()
	bool CanShoot = true;

public:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ABaseEnemyCharacter* GetEnemyCharacter();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EEnemyType GetEnemyType() { return GetEnemyCharacter()->GetEnemyType(); }

	UFUNCTION(BlueprintCallable)
	void Shoot(FVector Target, float Velocity);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetCanShoot() { return CanShoot; }

	UFUNCTION(BlueprintCallable)
	void CanShootEnable() { CanShoot = true; }

	UFUNCTION(BlueprintCallable)
	void CanShootDisable() { CanShoot = false;}

	virtual void Tick(float DeltaTime) override;
};
