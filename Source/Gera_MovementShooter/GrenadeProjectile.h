// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthComponent.h"
#include "Engine/DamageEvents.h"
#include "GrenadeProjectile.generated.h"



UCLASS()
class GERA_MOVEMENTSHOOTER_API AGrenadeProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrenadeProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxDamage = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* GrenadeMesh = nullptr;

	UFUNCTION(BlueprintCallable)
	void DealDamage(AActor* Collider, AActor* PlayerCharacter, AController* PlayerController);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
