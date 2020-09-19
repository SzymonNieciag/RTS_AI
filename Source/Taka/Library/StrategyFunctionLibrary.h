// DRAGO Entertainment (c) 2019+

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StrategyFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class TAKA_API UStrategyFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "AI|Task")
	static class UTaskBase* SpawnNextAITask(AAICharacterBase *AICharacter);

};
