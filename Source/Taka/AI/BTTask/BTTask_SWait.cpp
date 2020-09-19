// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SWait.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "../AIControllerBase.h"
#include "Taka/Characters/AICharacterBase.h"

UBTTask_SWait::UBTTask_SWait(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WaitTime = 5.0f;
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_SWait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	RemainingWaitTime = FMath::FRandRange(FMath::Max(0.0f, WaitTime - RandomDeviation), (WaitTime + RandomDeviation));
	
	return EBTNodeResult::InProgress;
}

void UBTTask_SWait::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	UBlackboardData* BlackboardData = GetBlackboardAsset();
	if (ensure(BlackboardData))
	{
		CurrentAIState.ResolveSelectedKey(*BlackboardData);
	}
}

void UBTTask_SWait::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	RemainingWaitTime -= DeltaSeconds;

	if (RemainingWaitTime <= 0.0f)
	{
		// continue execution from this node
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

		AAICharacterBase * AICharacterBase = Cast<AAICharacterBase>(OwnerComp.GetAIOwner()->GetPawn());

		AICharacterBase->CurrentActiveTask->OnStartTaskCompleted.Broadcast(ETaskState::Finish);
	}
}
