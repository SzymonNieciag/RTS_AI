// DRAGO Entertainment (c) 2019+

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WayPoint.generated.h"

UCLASS()
class TAKA_API AWayPoint : public AActor
{
	GENERATED_BODY()
	
public:

	AWayPoint();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StrategyActor")
	UStaticMeshComponent* MainMesh;
	// Sets default values for this actor's properties

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

};
