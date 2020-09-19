// DRAGO Entertainment (c) 2019+	

#pragma once

#include "CoreMinimal.h"
#include "../TaskBase.h"
#include "SWait.generated.h"

UCLASS(Blueprintable)
class TAKA_API USWait : public UTaskBase
{
	GENERATED_BODY()
	
public:

	USWait();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Task")
	float Time = 20;

	virtual void InitializeTask_Implementation(class AAICharacterBase* Character, class AActor * NewTargetActor) override;
	virtual void StartTask_Implementation() override;

	void Tick(float DeltaTime) override;

};
