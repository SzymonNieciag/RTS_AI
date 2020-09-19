// DRAGO Entertainment (c) 2019+


#include "TaskBase.h"
#include "../Characters/AICharacterBase.h"
#include "AIControllerBase.h"

UWorld* UTaskBase::GetWorld() const
{
	if (AICharacter) 
	return AICharacter->GetWorld();
    else 
	return nullptr;
}

UTaskBase::UTaskBase()
{
	bIsCreateOnRunning = GIsRunning;
}

UTaskBase::~UTaskBase()
{

}

void UTaskBase::InitializeTask_Implementation(class AAICharacterBase* NewCharacter, class AActor * NewTarget)
{
	AICharacter = NewCharacter;
	AICharacter->CurrentTargetActor = NewTarget;
	AICharacter->CurrentActiveTask = this;
}

void UTaskBase::StartTask_Implementation()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("StartAction"));   
}

void UTaskBase::FinishTask()
{
	AICharacter->CurrentTargetActor = nullptr;
	AICharacter->CurrentActiveTask = nullptr;
	SetTickEnabled(false);
	ConditionalBeginDestroy();
}

void UTaskBase::Tick(float DeltaTime)
{
	if (!this->IsUnreachable())
	{
		TickObject(DeltaTime);
	}
}

bool UTaskBase::IsTickable() const
{
	// Tick only if we are both NOT a template and if we are specifically not in-editor-before-beginplay is called.
	return (!IsTemplate(RF_ClassDefaultObject) && TickEnabled);
}

TStatId UTaskBase::GetStatId() const
{
	return UObject::GetStatID();
}

void UTaskBase::SetTickEnabled(bool Enable)
{
	TickEnabled = Enable;
}

