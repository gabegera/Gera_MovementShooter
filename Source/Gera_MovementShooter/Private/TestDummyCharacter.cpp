// Fill out your copyright notice in the Description page of Project Settings.


#include "TestDummyCharacter.h"

// Sets default values
ATestDummyCharacter::ATestDummyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

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
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (HealthComponent)
	{
		HealthComponent->TakeDamage(Damage);
		
		FString DebugMessage = "Test Dummy Took: ";
		DebugMessage.Append(FString::SanitizeFloat(Damage));
		DebugMessage.Append(" Damage.");
		
		FString HealthMessage = "Test Dummy Has: ";
		HealthMessage.Append(FString::SanitizeFloat(HealthComponent->GetHealth()));
		HealthMessage.Append(" Health.");
		
		GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Red, DebugMessage);
		GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, HealthMessage);
	}
	return Damage;
}

