// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_CombatMovement.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "../AIControllerBase.h"
#include "Taka/Characters/AICharacterBase.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"

UBTT_CombatMovement::UBTT_CombatMovement(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_CombatMovement::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AICharacterBase = Cast<AAICharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	MovementRange = 500.0f;
	return EBTNodeResult::InProgress;
}

void UBTT_CombatMovement::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	/*UBlackboardData* BlackboardData = GetBlackboardAsset();
	if (ensure(BlackboardData))
	{
		CombatState.ResolveSelectedKey(*BlackboardData);
	}*/
}

void UBTT_CombatMovement::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
