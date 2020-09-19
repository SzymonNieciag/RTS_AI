// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_CombatRange.h"
#include <BehaviorTree/Blackboard/BlackboardKeyType_Enum.h>
#include <Taka/RTSDefines.h>
#include <Taka/TakaGameState.h>
#include <AIController.h>
#include <Taka/Characters/AICharacterBase.h>
#include <BehaviorTree/BlackboardComponent.h>


void UBTS_CombatRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
}

void UBTS_CombatRange::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
}

void UBTS_CombatRange::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);

	CheckDistanceToEnemy(SearchData.OwnerComp);
}

void UBTS_CombatRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	CheckDistanceToEnemy(OwnerComp);
}

void UBTS_CombatRange::CheckDistanceToEnemy(UBehaviorTreeComponent& OwnerComp)
{
	AAICharacterBase* OwnerCharacterBase = OwnerCharacterBase = Cast<AAICharacterBase>(OwnerComp.GetAIOwner()->GetPawn());

	const float DistanceToEnemy = FVector::Distance(OwnerCharacterBase->GetActorLocation(), OwnerCharacterBase->CurrentEnemyCharacter->GetActorLocation());

	if (DistanceToEnemy > OwnerCharacterBase->CombatMovementRange)
	{
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(CombatState.GetSelectedKeyID(), (uint8)EAIState::Move);
	}
}

void UBTS_CombatRange::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		CombatState.ResolveSelectedKey(*BBAsset);
	}
	bNotifyBecomeRelevant = true;
}
