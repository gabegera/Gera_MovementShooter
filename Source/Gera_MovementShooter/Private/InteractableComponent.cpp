// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableComponent.h"


// Sets default values for this component's properties
UInteractableComponent::UInteractableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractableComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInteractableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UInteractableComponent::CheckIsInteractable_Implementation()
{
	return IsInteractable;
}

bool UInteractableComponent::Interact_Implementation(AActor* InteractCauser)
{
	switch (InteractableType)
	{
	case EInteractableType::InteractableObject:
		return Execute_Interact(GetOwner(), InteractCauser);
	case EInteractableType::InteractionTrigger:
		return Execute_Interact(InteractionReceiver, InteractCauser);
	default:
		return false;
	}
}

