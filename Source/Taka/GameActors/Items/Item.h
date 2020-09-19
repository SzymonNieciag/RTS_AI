// DRAGO Entertainment (c) 2019+

#pragma once

#include "CoreMinimal.h"
#include "../StrategyActor.h"
#include "Item.generated.h"

UCLASS()
class TAKA_API AItem : public AStrategyActor
{
	GENERATED_BODY()
	
public:
	AItem(const FObjectInitializer& ObjectInitializer);

	// Sets default values for this actor's properties

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

public:

#pragma region Interfaces|Selectable

	virtual bool BeforeSelect_Implementation(APlayerController *PlayerController) override;

	virtual bool Select_Implementation(APlayerController *PlayerController) override;

	virtual bool UnSelect_Implementation(APlayerController *PlayerController) override;

	const EStrategyUnitType GetStrategyUnitType_Implementation() override;

#pragma endregion Interfaces|Selectable

};
