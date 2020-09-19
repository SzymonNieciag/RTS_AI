// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_StartDetection.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "../AIControllerBase.h"
#include "Taka/Characters/AICharacterBase.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"

UBTTask_StartDetection::UBTTask_StartDetection(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_StartDetection::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AICharacterBase = Cast<AAICharacterBase>(OwnerComp.GetAIOwner()->GetPawn());

	AICharacterBase->SightStruct.DetectionProgress;

	return EBTNodeResult::InProgress;
}

void UBTTask_StartDetection::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	UBlackboardData* BlackboardData = GetBlackboardAsset();
	if (ensure(BlackboardData))
	{
		CombatState.ResolveSelectedKey(*BlackboardData);
	}
}

void UBTTask_StartDetection::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (AICharacterBase->CurrentEnemyCharacter = nullptr)
	{
		return;
	}

	const float SqrDistance = FVector::DistSquared(AICharacterBase->CurrentEnemyCharacter->GetActorLocation(), AICharacterBase->GetActorLocation());
	const float CriticRangeOfView = AICharacterBase->SightStruct.RangeOfView / 2;
	const float SqrCriticRangeOfView = CriticRangeOfView * CriticRangeOfView;

	if (SqrDistance <= SqrCriticRangeOfView)
	{
		AICharacterBase->SightStruct.DetectionProgress = 100.0f;

		AICharacterBase->OnDetectionUpdated.Broadcast(AICharacterBase->SightStruct.DetectionProgress, true);
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(CombatState.GetSelectedKeyID(), (uint8)ECombatMode::Attack);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		const float Formula = 1 + 5 * (1 - (SqrDistance / AICharacterBase->SightStruct.RangeOfView * AICharacterBase->SightStruct.RangeOfView));

		AICharacterBase->SightStruct.DetectionProgress += DeltaSeconds * 10 * Formula;

		if (AICharacterBase->SightStruct.DetectionProgress >= 100.0f)
		{
			AICharacterBase->OnDetectionUpdated.Broadcast(AICharacterBase->SightStruct.DetectionProgress, true);
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(CombatState.GetSelectedKeyID(), (uint8)ECombatMode::Attack);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
		else
		{
			AICharacterBase->OnDetectionUpdated.Broadcast(AICharacterBase->SightStruct.DetectionProgress, false);
		}
	}
}
