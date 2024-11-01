// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

#include "BehaviorTree/BlackboardComponent.h"

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	
	// switch (GetEnemyType())
	// {
	// case EEnemyType::Melee:
	// 	RunBehaviorTree(MeleeBehaviorTree);
	// 	break;
	// case EEnemyType::Ranged:
	// 	RunBehaviorTree(RangedBehaviorTree);
	// 	break;
	// }
	//
	// GetBlackboardComponent()->SetValueAsObject(FName("Target"), GetPawn());
	// GetBlackboardComponent()->SetValueAsVector(FName("TargetLocation"), GetPawn()->GetActorLocation());
}

AEnemyCharacter* AEnemyAIController::GetEnemyCharacter()
{
	if (!EnemyCharacter)
	{
		EnemyCharacter = Cast<AEnemyCharacter>(GetCharacter());
	}

	return EnemyCharacter;

}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// GetBlackboardComponent()->SetValueAsVector(FName("TargetLocation"), GetPawn()->GetActorLocation());
}
