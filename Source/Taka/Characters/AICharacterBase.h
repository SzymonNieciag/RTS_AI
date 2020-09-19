// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.h"
#include "../Interfaces/Selectable.h"
#include "AICharacterBase.generated.h"

USTRUCT(BlueprintType)
struct TAKA_API FSTaskStruct
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Task")
	TSubclassOf<class UTaskBase> TaskBase;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Task")
	class AActor * TargetActor;

	FSTaskStruct() : TaskBase(NULL), TargetActor(NULL) {}
	FSTaskStruct(TSubclassOf<class UTaskBase> NewTaskClass, class AActor * NewTargetActor) : TaskBase(NewTaskClass), TargetActor(NewTargetActor) {}
};

USTRUCT(BlueprintType)
struct TAKA_API FSightStruct
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Task")
	float AngleOfView = 120.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Task")
	float RangeOfView = 500.0f;

	/*From 1 to 100*/
	float DetectionProgress = 0.0f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDetectionUpdate, const float, Progress, const bool, DetectionFinish);

UCLASS(Blueprintable)
class AAICharacterBase : public ACharacterBase, public ISelectable
{
	GENERATED_BODY()

public:

	AAICharacterBase(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	// ** Detection ** //
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Detection")
	FDetectionUpdate OnDetectionUpdated;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Detection")
	class UProceduralMeshComponent * ProceduralViewOfView;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
	FSightStruct SightStruct;

public:

	UFUNCTION(BlueprintCallable, Category = "Detection")
	class UProceduralMeshComponent * DrawVieldOfView();

private:

	void GetViewPoints(TArray<FVector>& OutPoints);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task")
	EStrategyUnitType StrategyUnitType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	EAITeam IDTeam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task")
	TArray<FSTaskStruct> STasks;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Task")
	class UTaskBase* CurrentActiveTask;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Task")
	AActor* CurrentTargetActor;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Task")
	class AAICharacterBase* CurrentEnemyCharacter;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Task")
	float CombatMovementRange = 300.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Task")
	float InteractRange = 100.0f;
	
#pragma region Interfaces|Selectable

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interfaces|Interaction")
	bool BeforeSelect(APlayerController *PlayerController);
	virtual bool BeforeSelect_Implementation(APlayerController *PlayerController);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interfaces|Interaction")
	bool Select(APlayerController *PlayerController);
	virtual bool Select_Implementation(APlayerController *PlayerController);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interfaces|Interaction")
	bool UnSelect(APlayerController *PlayerController);
	virtual bool UnSelect_Implementation(APlayerController *PlayerController);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interfaces|Interaction")
	const EStrategyUnitType GetStrategyUnitType();
	const EStrategyUnitType GetStrategyUnitType_Implementation();

#pragma endregion Interfaces|Selectable

public:
	
	UFUNCTION(BlueprintPure, Category = "Character")
	const FVector GetCharacterLocation();

	UFUNCTION(BlueprintCallable, Category = "Character")
	void AddTaskToQueue(TSubclassOf<class UTaskBase> NewTask, AActor * TargetActor = nullptr, bool HasPriority = false);

	UFUNCTION(BlueprintCallable, Category = "Character")
	void AbortTasks();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interfaces|Interaction")
	const EAITeam GetTeam();
	const EAITeam GetTeam_Implementation();

};