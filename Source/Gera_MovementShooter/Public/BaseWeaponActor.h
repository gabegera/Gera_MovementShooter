// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTables/WeaponData.h"
#include "GameFramework/Actor.h"
#include "BaseWeaponActor.generated.h"

UCLASS()
class GERA_MOVEMENTSHOOTER_API ABaseWeaponActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseWeaponActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;

	// Arrow Component used to find the location of the weapons muzzle.
	UPROPERTY(EditAnywhere)
	class UArrowComponent* MuzzleArrow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(RowType="WeaponData"))
	FDataTableRowHandle WeaponDataTableRowHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(EditCondition=false))
	FWeaponData WeaponData;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
