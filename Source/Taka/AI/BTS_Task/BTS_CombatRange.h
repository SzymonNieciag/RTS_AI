// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include <BehaviorTree/BehaviorTreeComponent.h>
#include "BTS_CombatRange.generated.h"

/**
 * 
 */
class AAICharacterBase;

UCLASS()
class TAKA_API UBTS_CombatRange : public UBTService
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector CombatState;

	/** called when auxiliary node becomes active
	* this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual void OnBecomeRelevant(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;

	/** called when auxiliary node becomes inactive
	* this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual void OnCeaseRelevant(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)override;

	virtual void OnSearchStart(FBehaviorTreeSearchData &SearchData)override;

	virtual void TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds) override;

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

private:

	void CheckDistanceToEnemy(UBehaviorTreeComponent& OwnerComp);

};
