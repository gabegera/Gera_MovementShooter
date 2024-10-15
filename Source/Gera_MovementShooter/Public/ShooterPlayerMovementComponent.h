// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ShooterPlayerMovementComponent.generated.h"

UENUM()
enum ECustomMovementMode
{
	CMOVE_None			UMETA(Hidden),
	CMOVE_Crouched		UMETA(DisplayName = "Crouched"),
	CMOVE_Slide			UMETA(DisplayName = "Slide"),
	CMOVE_Dash			UMETA(DisplayName = "Dash"),
};

UCLASS()
class GERA_MOVEMENTSHOOTER_API UShooterPlayerMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
};
