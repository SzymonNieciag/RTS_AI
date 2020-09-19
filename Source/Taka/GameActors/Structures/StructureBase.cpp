// DRAGO Entertainment (c) 2019+


#include "StructureBase.h"

AStructureBase::AStructureBase(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

void AStructureBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AStructureBase::Destroyed()
{
	Super::Destroyed();

}

bool AStructureBase::TakeActor(AAICharacterBase * AICharacterBase)
{
	return Super::TakeActor(AICharacterBase);
}

bool AStructureBase::BeforeSelect_Implementation(APlayerController *PlayerController)
{
	return Super::BeforeSelect_Implementation(PlayerController);
}

bool AStructureBase::Select_Implementation(APlayerController *PlayerController)
{
	return Super::Select_Implementation(PlayerController);
}

bool AStructureBase::UnSelect_Implementation(APlayerController *PlayerController)
{
	return Super::UnSelect_Implementation(PlayerController);
}

const EStrategyUnitType AStructureBase::GetStrategyUnitType_Implementation()
{
	return Super::GetStrategyUnitType_Implementation();
}