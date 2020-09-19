// DRAGO Entertainment (c) 2019+


#include "SMoveTo.h"
#include "NavigationSystem.h"
#include <Taka/Characters/AICharacterBase.h>
#include <Kismet/KismetMathLibrary.h>
#include <Taka/Pathfinding/WayPoint.h>
#include <NavigationPath.h>
#include <GameFramework/Actor.h> 

USMoveTo::USMoveTo()
{
	SetTickEnabled(false);
}

void USMoveTo::InitializeTask_Implementation(class AAICharacterBase* Character, AActor * NewTargetActor)
{
	Super::InitializeTask_Implementation(Character, NewTargetActor);
}

void USMoveTo::StartTask_Implementation()
{
	Super::StartTask_Implementation();

	AWayPoint * WayPoint = nullptr;

	if (AICharacter->CurrentTargetActor)
	{
		WayPoint = Cast<AWayPoint>(AICharacter->CurrentTargetActor);
	}
	else
	{
		WayPoint = nullptr;
	}

	if (WayPoint)
	{
		UNavigationPath * CheckNavigationPath = UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), AICharacter->GetActorLocation(), WayPoint->GetActorLocation());
		if (CheckNavigationPath && CheckNavigationPath->IsPartial())
		{
			WayPoint->K2_DestroyActor();
			OnStartTaskCompleted.Broadcast(ETaskState::Finish);
			return;
		}
		float Distance = UKismetMathLibrary::Vector_Distance(AICharacter->GetCharacterLocation(), WayPoint->GetActorLocation());

		if (Distance <= AICharacter->InteractRange)
		{
			WayPoint->K2_DestroyActor();
			OnStartTaskCompleted.Broadcast(ETaskState::Finish);
			return;
		}
		else
		{
			AICharacter->CurrentTargetActor = WayPoint;
			OnStartTaskCompleted.Broadcast(ETaskState::Move);
		}
	}
	else
	{
		OnStartTaskCompleted.Broadcast(ETaskState::Finish);
	}
}

void USMoveTo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
