// DRAGO Entertainment (c) 2019+	

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Tickable.h"
#include "TaskBase.generated.h"

UENUM(BlueprintType)
enum class ETaskState :uint8
{
	Action = 0 UMETA(DisplayName = "Action"),
	Move = 1 UMETA(DisplayName = "Move"),
	NoAction = 2 UMETA(DisplayName = "NoAction"),
	Finish = 3 UMETA(DisplayName = "Finish"),
};

UENUM(BlueprintType)
enum class ETaskResult :uint8
{
	Completed = 0 UMETA(DisplayName = "Completed"),
	Aborted = 1 UMETA(DisplayName = "Aborted"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStartTaskCompleted, const ETaskState, TaskState);

UCLASS(Blueprintable, Abstract)
class TAKA_API UTaskBase : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
	
public:

	UTaskBase();
	virtual ~UTaskBase();

	virtual class UWorld* GetWorld() const override;

	UPROPERTY(BlueprintReadWrite, Category = "Task")
	class AAICharacterBase * AICharacter;

	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "InitializeTask"))
	void InitializeTask(class AAICharacterBase* NewCharacter, class AActor * NewTarget = nullptr);
	virtual void InitializeTask_Implementation(class AAICharacterBase* Character, class AActor * TargetActor = nullptr);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "StartTask"))
	void StartTask();
	virtual void StartTask_Implementation();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "StartTask"))
	virtual void FinishTask();

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Task")
	FStartTaskCompleted OnStartTaskCompleted;

#pragma region TickableGameObject

public:

	// Begin FTickableGameObject Interface.
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;
	// End FTickableGameObject Interface.

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Tick"))
	void TickObject(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Tick")
	void SetTickEnabled(bool Enable);

private:

	//Because engine would construct inner object when game load package (before game start), so we need to add a flag to identify which one is construct on game running.
	bool bIsCreateOnRunning = false;

	bool TickEnabled = false;

#pragma endregion TickableGameObject



};
