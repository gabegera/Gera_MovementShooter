// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterCharacter.h"
#include "Components/CapsuleComponent.h"

#include "ShooterPlayerCharacter.generated.h"



UCLASS()
class GERA_MOVEMENTSHOOTER_API AShooterPlayerCharacter : public AShooterCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterPlayerCharacter();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	/*UFUNCTION(BlueprintCallable)
	void Look(float InputX, float InputY);*/
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent = nullptr;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
};
