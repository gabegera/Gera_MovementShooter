// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CurrentHealth = MaxHealth;

	// ...
}

void UHealthComponent::TakeDamage(float Damage)
{
	CurrentHealth -= Damage;

	if (CurrentHealth < 0) CurrentHealth = 0;
}

void UHealthComponent::AddHealth(float Health)
{
	CurrentHealth += Health;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}



// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float HealthDrainMultiplier = CurrentHealth / MaxHealth;
	HealthDrainMultiplier *= HealthDrainMultiplier * 5;

	if (CurrentHealth > MaxHealth)
	{
		if (CurrentHealth - DeltaTime * HealthDrainMultiplier > MaxHealth)
		{
			CurrentHealth -= DeltaTime * HealthDrainMultiplier;
		}
		else if (CurrentHealth - DeltaTime * HealthDrainMultiplier < MaxHealth)
		{
			CurrentHealth = MaxHealth;
		}
	}
	


	// ...
}

