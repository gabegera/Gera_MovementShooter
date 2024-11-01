// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UPickupComponent::UPickupComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPickupComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...

	PlayerCharacter = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}


// Called every frame
void UPickupComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// ...
}

void UPickupComponent::SetOutlineColor(FLinearColor Color)
{
	if (!IsValid(OutlineMaterial)) return;
	
	if (!IsValid(DynamicOutline))
	{
		DynamicOutline = UMaterialInstanceDynamic::Create(OutlineMaterial, this);
	}
	
	DynamicOutline->SetVectorParameterValue(FName("Color"), Color);
}

