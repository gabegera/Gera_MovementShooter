// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoPickupObject.generated.h"

UENUM(BlueprintType)
enum class AmmoType : uint8
{
	RIFLE = 0 UMETA(DisplayName = "Rifle"),
	SHOTGUN = 1 UMETA(DisplayName = "Shotgun"),
	GRENADELAUNCHER = 2 UMETA(DisplayName = "Grenade Launcher"),
};

UCLASS()
class GERA_MOVEMENTSHOOTER_API AAmmoPickupObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmmoPickupObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* AmmoMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UMaterialInterface* RifleMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UMaterialInterface* ShotgunMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UMaterialInterface* GrenadeLauncherMaterial = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AmmoType AmmoType = AmmoType::RIFLE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int RifleAmmo = 20;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int ShotgunAmmo = 4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int GrenadeLauncherAmmo = 1;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
