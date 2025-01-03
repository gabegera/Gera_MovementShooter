// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTables/WeaponData.h"
#include "GameFramework/Actor.h"
#include "Interfaces/PickupInterface.h"
#include "Components/SphereComponent.h"
#include "DataTables/AmmoData.h"
#include "DataTables/ConsumableData.h"
#include "DataTables/EquipmentData.h"
#include "PickupObject.generated.h"

UENUM(BlueprintType)
enum class EPickupType : uint8
{
	Ammo = 0 UMETA(DisplayName = "Ammo"),
	Consumable = 1 UMETA(DisplayName = "Consumable"),
	Equipment = 2 UMETA(DisplayName = "Equipment"),
	Weapon = 3 UMETA(DisplayName = "Weapon")
};

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
	EPickupType PickupType = EPickupType::Ammo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition="PickupType == EPickupType::Ammo", EditConditionHides, RowType="AmmoData"))
	FDataTableRowHandle AmmoDataTableRowHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition="PickupType == EPickupType::Consumable", EditConditionHides, RowType="ConsumableData"))
	FDataTableRowHandle ConsumableDataTableRowHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition="PickupType == EPickupType::Equipment", EditConditionHides, RowType="EquipmentData"))
	FDataTableRowHandle EquipmentDataTableRowHandle;	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition="PickupType == EPickupType::Weapon", EditConditionHides, RowType="WeaponData"))
	FDataTableRowHandle WeaponDataTableRowHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* PickupStaticMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* PickupSkeletalMesh = nullptr;

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

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EConsumableEffect GetConsumableEffect() const { return ConsumableDataTableRowHandle.GetRow<FConsumableData>("")->ConsumableEffect; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FAmmoData GetAmmoData() const { return *AmmoDataTableRowHandle.GetRow<FAmmoData>(""); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FConsumableData GetConsumableData() const { return *ConsumableDataTableRowHandle.GetRow<FConsumableData>(""); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FEquipmentData GetEquipmentData() const { return *EquipmentDataTableRowHandle.GetRow<FEquipmentData>(""); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FWeaponData GetWeaponData() const { return *WeaponDataTableRowHandle.GetRow<FWeaponData>(""); }

};
