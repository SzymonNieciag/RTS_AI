// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_StartDetection.generated.h"


UCLASS()
class TAKA_API UBTTask_StartDetection : public UBTTaskNode
{
	GENERATED_UCLASS_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	/** initialize any asset related data */
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	/** blackboard key selector */
	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector CombatState;

protected:

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:

	class AAICharacterBase * AICharacterBase;

	float RemainingWaitTime = 0.0f;
};
