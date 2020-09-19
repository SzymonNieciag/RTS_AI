// DRAGO Entertainment (c) 2019+


#include "ResourceBase.h"

AResourceBase::AResourceBase(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

void AResourceBase::BeginPlay()
{
	Super::BeginPlay();

}

void AResourceBase::Destroyed()
{
	Super::Destroyed();

}

bool AResourceBase::BeforeSelect_Implementation(APlayerController *PlayerController)
{
	return Super::BeforeSelect_Implementation(PlayerController);
}

bool AResourceBase::Select_Implementation(APlayerController *PlayerController)
{
	return Super::Select_Implementation(PlayerController);
}

bool AResourceBase::UnSelect_Implementation(APlayerController *PlayerController)
{
	return Super::UnSelect_Implementation(PlayerController);
}

const EStrategyUnitType AResourceBase::GetStrategyUnitType_Implementation()
{
	return Super::GetStrategyUnitType_Implementation();
}