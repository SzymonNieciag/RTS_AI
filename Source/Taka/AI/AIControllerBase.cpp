// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerBase.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "../Characters/AICharacterBase.h"
#include "../Library/StrategyFunctionLibrary.h"

void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();
	InitiativeTask();
}

void AAIControllerBase::InitiativeTask_Implementation()
{
	AICharacter = Cast<AAICharacterBase>(GetPawn());

	if (AICharacter->CurrentActiveTask)
	{
		AICharacter->CurrentActiveTask->OnStartTaskCompleted.AddDynamic(this, &AAIControllerBase::StopInitiativeTask);
		AICharacter->CurrentActiveTask->StartTask();
	}
	else
	{
		UTaskBase * NewTask = UStrategyFunctionLibrary::SpawnNextAITask(AICharacter);
		if (NewTask)
		{
			NewTask->OnStartTaskCompleted.AddDynamic(this, &AAIControllerBase::StopInitiativeTask);
			AICharacter->CurrentActiveTask->StartTask();
		}
		else
		{
			SetState(EAIState::DefaultAction);
		}
	}
}

void AAIControllerBase::SetState(EAIState State /*= EAIState::Action*/)
{
	GetBlackboardComponent()->SetValueAsEnum("State", (UINT8)State);
}

EAIState AAIControllerBase::GetState()
{
	EAIState CurrentState = EAIState::ActionPriority;
	if (GetBlackboardComponent())
	{
		CurrentState = EAIState(GetBlackboardComponent()->GetValueAsEnum("State"));
	}
	return CurrentState;
}

void AAIControllerBase::StopInitiativeTask(const ETaskState TaskState)
{
	//AICharacter->CurrentActiveTask->OnStartTaskCompleted.RemoveDynamic(this, &AAIControllerBase::StopInitiativeTask);
	switch (TaskState)
	{
	case ETaskState::Action:
	{
		AICharacter->CurrentActiveTask->StartTask();
		SetState(EAIState::ActionPriority);
		break;
	}
	case ETaskState::Move:
	{
		SetState(EAIState::Move);
		break;
	}
	case ETaskState::NoAction:
	{
		SetState(EAIState::DefaultAction);
		break;
	}
	case ETaskState::Finish:
	{
		AICharacter->CurrentActiveTask->FinishTask();
		SetState(EAIState::ActionPriority);
		InitiativeTask();
		break;
	}
	}
}
