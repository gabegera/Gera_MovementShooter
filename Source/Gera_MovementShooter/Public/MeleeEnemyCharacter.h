// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemyCharacter.h"
#include "MeleeEnemyCharacter.generated.h"

UENUM(BlueprintType)
enum class EMeleeEnemyType : uint8
{
	Grunt = 0 UMETA(DisplayName = "Grunt"),
	Tank = 1 UMETA(DisplayName = "Tank"),
};


UCLASS()
class GERA_MOVEMENTSHOOTER_API AMeleeEnemyCharacter : public ABaseEnemyCharacter
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	EMeleeEnemyType GetEnemyType() const { return EnemyType; }

protected:

	UPROPERTY(EditDefaultsOnly, Category = "AI Settings")
	EMeleeEnemyType EnemyType = EMeleeEnemyType::Grunt;
	
};
