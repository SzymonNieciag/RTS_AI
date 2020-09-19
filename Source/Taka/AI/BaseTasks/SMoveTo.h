// DRAGO Entertainment (c) 2019+	

#pragma once

#include "CoreMinimal.h"
#include "../TaskBase.h"
#include "SMoveTo.generated.h"

UCLASS(Blueprintable)
class TAKA_API USMoveTo : public UTaskBase
{
	GENERATED_BODY()
	
public:

	USMoveTo();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Task")
	float Radius = 50;

	virtual void InitializeTask_Implementation(class AAICharacterBase* Character, class AActor * NewTargetActor) override;
	virtual void StartTask_Implementation() override;

	void Tick(float DeltaTime) override;

};
