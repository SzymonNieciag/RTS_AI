// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "../RTSDefines.h"
#include "TaskBase.h"
#include "AIControllerBase.generated.h"

UCLASS()
class TAKA_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()

public:
	
	virtual void BeginPlay() override;

	UPROPERTY(BLueprintReadOnly, Category = "RTS Task")
	class AAICharacterBase * AICharacter;

	UFUNCTION(BlueprintNativeEvent, Category = "RTSTask")
	void InitiativeTask();
	virtual void InitiativeTask_Implementation();

	UFUNCTION()
	void StopInitiativeTask(const ETaskState TaskState);

	void SetState(EAIState State = EAIState::ActionPriority);
	EAIState GetState();
};