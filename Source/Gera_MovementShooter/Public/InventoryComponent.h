// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTables/WeaponData.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class ABaseWeaponActor;



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GERA_MOVEMENTSHOOTER_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void PostInitProperties() override;

	UPROPERTY(EditDefaultsOnly, meta=(RowStructure="Gera_MovementShooter.WeaponData"))
	UDataTable* WeaponsDataTable;

	UPROPERTY(EditDefaultsOnly, meta=(ToolTip="Click to Refresh list of Starting Weapons. Use whenever changes are made to the Weapons Data Table."))
	bool RefreshWeaponsDataTable;

	UPROPERTY(EditDefaultsOnly)
	TMap<FName, bool> StartingWeapons;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(RowType="WeaponData"))
	TArray<FWeaponData> Weapons;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EAmmoType, int> AmmoMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, int> EquipmentMap;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddWeapon(FWeaponData NewWeapon);

	void AddAmmo(EAmmoType AmmoType, int32 AmmoAmount);

	TArray<FWeaponData>* GetWeapons() { return &Weapons; }
	
};
