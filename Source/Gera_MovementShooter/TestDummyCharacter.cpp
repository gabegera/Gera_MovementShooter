// Fill out your copyright notice in the Description page of Project Settings.


#include "TestDummyCharacter.h"

// Sets default values
ATestDummyCharacter::ATestDummyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	

}

// Called when the game starts or when spawned
void ATestDummyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestDummyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATestDummyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ATestDummyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// TODO: Change Damage Text

	// TODO: Add Hit Reaction Animation

	return 0.0f;
}

