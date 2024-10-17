// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponData.h"
#include "ItemData.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


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

	UPROPERTY(EditDefaultsOnly)
	UDataTable* WeaponsDataTable = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle EmptyHands;

	UPROPERTY(BlueprintReadOnly)
	TMap<EAmmoType, int> AmmoMap;

	UPROPERTY(BlueprintReadOnly)
	TMap<FName, int> EquipmentMap;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FDataTableRowHandle PrimaryWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FDataTableRowHandle SecondaryWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FDataTableRowHandle HeavyWeapon;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FDataTableRowHandle GetEmptyHands() { return EmptyHands; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FDataTableRowHandle EquipmentSlot;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FItemData GetEquippedEquipmentData() { return *EquipmentSlot.GetRow<FItemData>(""); }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FDataTableRowHandle SupportItemSlot;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FItemData GetEquippedSupportData() { return *SupportItemSlot.GetRow<FItemData>(""); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetAmmo(EAmmoType AmmoType);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetEquipment(FName EquipmentName);
	
	UFUNCTION(BlueprintCallable)
	void AddAmmo(EAmmoType AmmoType, int AmmoAmount);

	UFUNCTION(BlueprintCallable)
	void AddEquipment(FName EquipmentName, int EquipmentAmount);

	UFUNCTION(BlueprintCallable)
	void RemoveAmmo(EAmmoType AmmoType, int AmmoAmount);

	UFUNCTION(BlueprintCallable)
	void RemoveEquipment(FName EquipmentName, int EquipmentAmount);

	UFUNCTION(BlueprintCallable)
	void SwapWeapons(EWeaponSlot WeaponSlot, FDataTableRowHandle NewWeapon);

	UFUNCTION(BlueprintCallable)
	void SwapEquipment(FDataTableRowHandle NewEquipment);

	UFUNCTION(BlueprintCallable)
	void SwapSupportItem(FDataTableRowHandle NewBuffItem);
	
};
