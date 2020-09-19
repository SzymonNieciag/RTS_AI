// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SWait.generated.h"


UCLASS()
class TAKA_API UBTTask_SWait : public UBTTaskNode
{
	GENERATED_UCLASS_BODY()

	/** wait time in seconds */
	UPROPERTY(Category = Wait, EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float WaitTime;

	/** allows adding random time to wait time */
	UPROPERTY(Category = Wait, EditAnywhere, meta = (UIMin = 0, ClampMin = 0))
	float RandomDeviation;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	
	/** initialize any asset related data */
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

protected:

	/** blackboard key selector */
	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector CurrentAIState;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:

	float RemainingWaitTime;

};
