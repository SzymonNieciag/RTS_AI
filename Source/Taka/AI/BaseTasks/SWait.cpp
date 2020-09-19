// DRAGO Entertainment (c) 2019+


#include "SWait.h"

USWait::USWait() 
{

}

void USWait::InitializeTask_Implementation(class AAICharacterBase* Character, class AActor * NewTargetActor)
{
	Super::InitializeTask_Implementation(Character, NewTargetActor);
}

void USWait::StartTask_Implementation()
{
	Super::StartTask_Implementation();
	OnStartTaskCompleted.Broadcast(ETaskState::NoAction);
}

void USWait::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
