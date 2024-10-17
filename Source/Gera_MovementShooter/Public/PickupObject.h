// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupComponent.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "PickupObject.generated.h"



UCLASS()
class GERA_MOVEMENTSHOOTER_API APickupObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void BeginPickupSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPickupComponent* PickupComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USphereComponent* InteractSphereTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USphereComponent* PickupSphereTrigger;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* PickupMesh = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void RefreshPickup();

	UFUNCTION(BlueprintCallable)
	FDataTableRowHandle SetWeaponPickup(FDataTableRowHandle NewWeapon);

	UFUNCTION(BlueprintCallable)
	FDataTableRowHandle SetItemPickup(FDataTableRowHandle NewItem);

};
