// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HealthComponent.h"
#include "InventoryComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "ShooterPlayerCharacter.generated.h"


UCLASS()
class GERA_MOVEMENTSHOOTER_API AShooterPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterPlayerCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UChildActorComponent* WeaponChildComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInventoryComponent* InventoryComponent = nullptr;

	UPROPERTY(BlueprintReadOnly)
	FDataTableRowHandle EquippedWeapon;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FWeaponData GetEquippedWeaponData() { return *EquippedWeapon.GetRow<FWeaponData>(""); }
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	/*UFUNCTION(BlueprintCallable)
	void Look(float InputX, float InputY);*/

	UPROPERTY(EditAnywhere)
	UHealthComponent* HealthComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* WeaponChildMesh = nullptr;

	UFUNCTION(BlueprintCallable)
	void EquipWeapon(FDataTableRowHandle Weapon);

	UPROPERTY()
	TSet<AActor*> PickupSet;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AActor* FindClosestPickup();

	UFUNCTION(BlueprintCallable)
	void PickupItem();
};
