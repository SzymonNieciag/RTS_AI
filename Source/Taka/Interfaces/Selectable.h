// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../RTSDefines.h"
#include "Selectable.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class TAKA_API USelectable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TAKA_API ISelectable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ISelectable")
	bool BeforeSelect(APlayerController *PlayerController);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ISelectable")
	bool Select(APlayerController *PlayerController);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ISelectable")
	bool UnSelect(APlayerController *PlayerController);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ISelectable")
	const EStrategyUnitType GetStrategyUnitType();
};
