// DRAGO Entertainment (c) 2019+


#include "StrategyActor.h"
#include "Components/StaticMeshComponent.h"
#include <Components/ArrowComponent.h>

AStrategyActor::AStrategyActor(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	RootComponent = MainMesh;

	DestinateTransform = CreateDefaultSubobject<UArrowComponent>(TEXT("GoalArrowComponent"));
	DestinateTransform->SetupAttachment(RootComponent);
}

void AStrategyActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AStrategyActor::Destroyed()
{
	Super::Destroyed();

}

bool AStrategyActor::TakeActor(AAICharacterBase * AICharacterBase)
{
	if (AICharacters.Num() < MaxUsers)
	{
		AICharacters.Add(AICharacterBase);
		return true;
	}
	return false;
}

bool AStrategyActor::LeaveActor(AAICharacterBase * AICharacterBase)
{
	if (AICharacters.Num() < MaxUsers)
	{
		AICharacters.Remove(AICharacterBase);
		return true;
	}
	return false;
}

bool AStrategyActor::BeforeSelect_Implementation(APlayerController *PlayerController)
{
	return true;
}

bool AStrategyActor::Select_Implementation(APlayerController *PlayerController)
{
	MainMesh->SetRenderCustomDepth(true);
	return true;
}

bool AStrategyActor::UnSelect_Implementation(APlayerController *PlayerController)
{
	MainMesh->SetRenderCustomDepth(false);
	return true;
}

TSubclassOf<class UTaskBase> AStrategyActor::GetInteractableTask_Implementation(AAICharacterBase * AICharacterBase)
{
	TSubclassOf<UTaskBase> ReturnTask = nullptr;
	if(Tasks.Num() != 0)
	{
		ReturnTask = Tasks[0];
	}
	return ReturnTask;
}


const EStrategyUnitType AStrategyActor::GetStrategyUnitType_Implementation()
{
	return StrategyUnitType;
}

const FTransform AStrategyActor::GetInteractableTransform_Implementation(AAICharacterBase * AICharacterBase)
{
	FTransform Destination;
	if (DestinateTransform)
	{
		Destination = DestinateTransform->GetComponentTransform();
	}
	else
	{
		Destination = this->GetTransform();
	}
	return Destination;
}
