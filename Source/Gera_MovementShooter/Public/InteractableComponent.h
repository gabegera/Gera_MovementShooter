// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/InteractInterface.h"
#include "InteractableComponent.generated.h"

// Used to choose whether the current Actor is being interacted with or if it's a trigger to Activate an Interaction on a separate Actor.
UENUM(BlueprintType)
enum class EInteractableType : uint8
{
	InteractableObject = 1 UMETA(DisplayName = "InteractableObject"),
	InteractionTrigger = 2 UMETA(DisplayName = "InteractionTrigger"),
};

// The Action that will occur when the Actor is Interacted With.
UENUM(BlueprintType)
enum class EInteractableAction : uint8
{
	PickupItem = 1 UMETA(DisplayName = "Pickup Item"),
	OpenCloseDoor = 2 UMETA(DisplayName = "Open/Close Door"),
	
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GERA_MOVEMENTSHOOTER_API UInteractableComponent : public UActorComponent, public IInteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractableComponent(); 

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Interaction")
	bool IsInteractable = true;

	UPROPERTY(EditAnywhere, Category="Interaction")
	EInteractableType InteractableType = EInteractableType::InteractableObject;

	// If the Interactable is a trigger, which object will be the target.
	UPROPERTY(EditAnywhere, Category="Interaction", meta=(EditCondition="InteractableType == EInteractableType::InteractionTrigger", EditConditionHides))
	AActor* InteractionReceiver;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool CheckIsInteractable_Implementation() override;

	UFUNCTION(BlueprintCallable)
	bool Interact_Implementation(AActor* InteractCauser) override;
};
