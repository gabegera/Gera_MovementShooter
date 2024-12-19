// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseShooterCharacter.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Components/CapsuleComponent.h"

#include "ShooterPlayerCharacter.generated.h"



UCLASS()
class GERA_MOVEMENTSHOOTER_API AShooterPlayerCharacter : public ABaseShooterCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterPlayerCharacter();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UCameraComponent* GetFirstPersonCameraComponent() const { return CameraComponent; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AShooterPlayerController* GetPlayerController();

	// UFUNCTION(BlueprintCallable)
	// void UpdateLook(FVector2D Input);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	AShooterPlayerController* PlayerController = nullptr;
	
	/*UFUNCTION(BlueprintCallable)
	void UpdateLook(float InputX, float InputY);*/
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAIPerceptionStimuliSourceComponent* PerceptionStimuliSourceComponent = nullptr;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
};
