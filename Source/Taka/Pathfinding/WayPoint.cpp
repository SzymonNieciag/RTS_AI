// DRAGO Entertainment (c) 2019+


#include "WayPoint.h"
#include "Components/StaticMeshComponent.h"

AWayPoint::AWayPoint()
{
	MainMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainMesh"));
	RootComponent = MainMesh;
}

void AWayPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWayPoint::Destroyed()
{
	Super::Destroyed();
}
