// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemyCharacter.h"
#include "RangedEnemyCharacter.generated.h"

UENUM(BlueprintType)
enum class ERangedEnemyType : uint8
{
	Soldier = 0 UMETA(DisplayName = "Soldier"),
	Shotgunner = 1 UMETA(DisplayName = "Shotgunner"),
	Sniper = 2 UMETA(DisplayName = "Sniper"),
	Heavy = 3 UMETA(DisplayName = "Heavy"),
};

UCLASS()
class GERA_MOVEMENTSHOOTER_API ARangedEnemyCharacter : public ABaseEnemyCharacter
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	ERangedEnemyType GetEnemyType() const { return EnemyType; }

	UFUNCTION(BlueprintCallable)
	void PickupWeapon(APickupObject* Pickup);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "AI Settings")
	ERangedEnemyType EnemyType = ERangedEnemyType::Soldier;
	
};
