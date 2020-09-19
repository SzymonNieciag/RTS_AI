// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_AIPerception.h"
#include <Taka/Characters/AICharacterBase.h>
#include "../AIControllerBase.h"
#include <Taka/RTSDefines.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <BehaviorTree/Blackboard/BlackboardKeyType_Enum.h>
#include <Taka/TakaGameState.h>
#include <BehaviorTree/Blackboard/BlackboardKeyType_Object.h>
#include "Kismet/KismetMathLibrary.h"

void UBTS_AIPerception::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
}

void UBTS_AIPerception::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
}

void UBTS_AIPerception::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);
	SightDetection(SearchData.OwnerComp);
}

void UBTS_AIPerception::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	SightDetection(OwnerComp);
}

void UBTS_AIPerception::SightDetection(UBehaviorTreeComponent& OwnerComp)
{
	ATakaGameState* const TakaGameState = GetWorld() != NULL ? GetWorld()->GetGameState<ATakaGameState>() : NULL;
	if (TakaGameState == nullptr)
	{
		return;
	}

	OwnerCharacterBase = Cast<AAICharacterBase>(OwnerComp.GetAIOwner()->GetPawn());
	SearchEnemy(TakaGameState);

	if (EnemyCharacterBase == nullptr)
	{
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(CombatState.GetSelectedKeyID(), (uint8)ECombatMode::Idle);
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(Enemy.GetSelectedKeyID(), nullptr);
		OwnerCharacterBase->SightStruct.DetectionProgress = 0.0f; 
		OwnerCharacterBase->CurrentEnemyCharacter = nullptr;
	}
	else if (EnemyCharacterBase && OwnerCharacterBase->SightStruct.DetectionProgress >= 100)
	{
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(CombatState.GetSelectedKeyID(), (uint8)ECombatMode::Attack);
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(Enemy.GetSelectedKeyID(), EnemyCharacterBase);
		OwnerCharacterBase->CurrentEnemyCharacter = EnemyCharacterBase;
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(CombatState.GetSelectedKeyID(), (uint8)ECombatMode::Searching);
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(Enemy.GetSelectedKeyID(), EnemyCharacterBase);
		OwnerCharacterBase->CurrentEnemyCharacter = EnemyCharacterBase;
	}
}

void UBTS_AIPerception::SearchEnemy(ATakaGameState* const TakaGameState)
{
	TArray<AAICharacterBase*> EnemyCharacters;
	for (int i = 0; i < TakaGameState->AICharacters.Num(); i++)
	{
		if (TakaGameState->AICharacters[i]->GetTeam() != OwnerCharacterBase->GetTeam())
			EnemyCharacters.Add(TakaGameState->AICharacters[i]);
	}

	TArray<AAICharacterBase*> EnemyCharactersInRange;

	float SquaredSightRange = OwnerCharacterBase->SightStruct.RangeOfView * OwnerCharacterBase->SightStruct.RangeOfView;
	float SightAngle = OwnerCharacterBase->SightStruct.AngleOfView / 2;

	for (auto* CurrentEnemyCharacter : EnemyCharacters)
	{
		float SquaredDistance = OwnerCharacterBase->GetSquaredDistanceTo(CurrentEnemyCharacter);
		if (SquaredDistance < SquaredSightRange)
		{
			FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(OwnerCharacterBase->GetActorLocation(), CurrentEnemyCharacter->GetActorLocation());
			FRotator DeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(OwnerCharacterBase->GetActorRotation(), Rotation);
			if (UKismetMathLibrary::Abs(DeltaRotator.Yaw) < SightAngle)
			{
				EnemyCharactersInRange.Add(CurrentEnemyCharacter);
			}
		}
	}

	float AveSquaredClosestEnemyCharacter = 1000000;

	EnemyCharacterBase = nullptr;

	for (auto* CurrentEnemyCharacter : EnemyCharactersInRange)
	{
		float SquaredClosestEnemyCharacter = OwnerCharacterBase->GetSquaredDistanceTo(CurrentEnemyCharacter);
		if (SquaredClosestEnemyCharacter < AveSquaredClosestEnemyCharacter)
		{
			AveSquaredClosestEnemyCharacter = SquaredClosestEnemyCharacter;
			EnemyCharacterBase = CurrentEnemyCharacter;
		}
	}
}

void UBTS_AIPerception::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		CombatState.ResolveSelectedKey(*BBAsset);
		Enemy.ResolveSelectedKey(*BBAsset);

		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("inizjalizacja w perception")));
	}
	bNotifyBecomeRelevant = true;
}
