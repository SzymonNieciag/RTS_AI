// DRAGO Entertainment (c) 2019+


#include "StrategyFunctionLibrary.h"
#include "../Characters/AICharacterBase.h"
#include "../AI/BaseTasks/SWait.h"

class UTaskBase* UStrategyFunctionLibrary::SpawnNextAITask(AAICharacterBase *AICharacter)
{
	UTaskBase* NewTask = nullptr;

	if(AICharacter->STasks.Num() == 0)
	{
		AICharacter->STasks.Add(FSTaskStruct(USWait::StaticClass(), nullptr));
	}
	TSubclassOf<UTaskBase> NewTaskClass = AICharacter->STasks[0].TaskBase;
	AActor * TargetActor = AICharacter->STasks[0].TargetActor;

	if (NewTaskClass == NULL)
	{
		NewTaskClass = USWait::StaticClass();
	}

	NewTask = NewObject<UTaskBase>(AICharacter, NewTaskClass);
	NewTask->InitializeTask(AICharacter, TargetActor);
	AICharacter->STasks.RemoveAt(0);

	return NewTask;
}
