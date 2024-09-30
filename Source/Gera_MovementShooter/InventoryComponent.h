// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponData.h"
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDataTable* WeaponsDataTable = nullptr;

	UFUNCTION(BlueprintCallable)
	void SwapWeapons(EWeaponSlot WeaponSlot, FDataTableRowHandle NewWeapon);

	UPROPERTY(BlueprintReadOnly)
	TMap<EAmmoType, int> AmmoMap;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FDataTableRowHandle PrimaryWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FDataTableRowHandle SecondaryWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FDataTableRowHandle HeavyWeapon;

	UFUNCTION(BlueprintCallable)
	int GetAmmo(EAmmoType AmmoType);
	
	UFUNCTION(BlueprintCallable)
	void AddAmmo(EAmmoType AmmoType, int AmmoAmount);

	UFUNCTION(BlueprintCallable)
	void RemoveAmmo(EAmmoType AmmoType, int AmmoAmount);
	
};
