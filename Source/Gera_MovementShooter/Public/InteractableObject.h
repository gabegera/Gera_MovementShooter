// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractInterface.h"
#include "InteractableObject.generated.h"

// Used to choose whether the current Actor is being interacted with or if it's a trigger to Activate an Interaction on a separate Actor.
UENUM(BlueprintType)
enum class EInteractableType : uint8
{
	InteractableObject = 0 UMETA(DisplayName = "InteractableObject"),
	InteractionTrigger = 1 UMETA(DisplayName = "InteractionTrigger"),
};

UCLASS()
class GERA_MOVEMENTSHOOTER_API AInteractableObject : public AActor, public IInteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	bool IsInteractable = true;
	
	UPROPERTY(EditAnywhere, Category="Interaction")
	EInteractableType InteractableType = EInteractableType::InteractableObject;

	// If the Interactable is a trigger, which object will be the target.
	UPROPERTY(EditAnywhere, Category="Interaction", meta=(EditCondition="InteractableType == EInteractableType::InteractionTrigger", EditConditionHides))
	AActor* InteractionReceiver = nullptr;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="Interaction")
	bool GetIsInteractable_Implementation() override { return IsInteractable; }

	UFUNCTION(BlueprintCallable, Category="Interaction")
	bool Interact_Implementation(AActor* InteractCauser) override;
};
