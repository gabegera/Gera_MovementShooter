// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "DataTables/WeaponData.h"
#include "GameFramework/Actor.h"
#include "Interfaces/PickupInterface.h"
#include "Components/SphereComponent.h"
#include "DataTables/PickupData.h"
#include "PickupObject.generated.h"

class AShooterPlayerCharacter;

UCLASS()
class GERA_MOVEMENTSHOOTER_API APickupObject : public AActor, public IPickupInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FDataTableRowHandle PickupDataTableRowHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* PickupMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* OutlineMaterialInstance = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USphereComponent* InteractSphereTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USphereComponent* PickupSphereTrigger;
	
	
	UFUNCTION()
	void BeginPickupSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY()
	AShooterPlayerCharacter* PlayerCharacter;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetOutlineColor(FLinearColor Color);
	
	UFUNCTION(BlueprintCallable)
	void RefreshPickup();

	UFUNCTION(BlueprintCallable)
	FPickupData GetPickupData() const { return *PickupDataTableRowHandle.GetRow<FPickupData>(""); }

	UFUNCTION(BlueprintCallable)
	EPickupType GetPickupType() const { return GetPickupData().PickupType; }

	UFUNCTION(BlueprintCallable)
	EConsumableEffect GetConsumableEffect()	const { return GetPickupData().ConsumableEffect; }

	UFUNCTION(BlueprintCallable)
	UStaticMesh* GetPickupMesh() const { return GetPickupData().PickupMesh; }

	UFUNCTION(BlueprintCallable)
	FColor GetOutlineColor() const { return GetPickupData().PickupOutlineColor; }

	/* Returns Weapon Pickups Data Table Row Handle */
	UFUNCTION(BlueprintCallable)
	FDataTableRowHandle GetWeaponDataTableRowHandle() const { return GetPickupData().WeaponDataTableRowHandle; }

	UFUNCTION(BlueprintCallable)
	EAmmoType GetAmmoType() const { return GetPickupData().AmmoType; }

	// UFUNCTION(BlueprintCallable)
	// FDataTableRowHandle SetWeaponPickup(FDataTableRowHandle NewWeapon);
	//
	// UFUNCTION(BlueprintCallable)
	// FDataTableRowHandle SetItemPickup(FDataTableRowHandle NewItem);

};
