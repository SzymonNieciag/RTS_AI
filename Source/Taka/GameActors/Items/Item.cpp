// DRAGO Entertainment (c) 2019+


#include "Item.h"

AItem::AItem(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.DoNotCreateDefaultSubobject(TEXT("GoalArrowComponent")))
{
	MainMesh->SetCollisionProfileName("Item");
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
}

void AItem::Destroyed()
{
	Super::Destroyed();
}

bool AItem::BeforeSelect_Implementation(APlayerController *PlayerController)
{
	return Super::BeforeSelect_Implementation(PlayerController);
}

bool AItem::Select_Implementation(APlayerController *PlayerController)
{
	return Super::Select_Implementation(PlayerController);
}

bool AItem::UnSelect_Implementation(APlayerController *PlayerController)
{
	return Super::UnSelect_Implementation(PlayerController);
}

const EStrategyUnitType AItem::GetStrategyUnitType_Implementation()
{
	return Super::GetStrategyUnitType_Implementation();
}