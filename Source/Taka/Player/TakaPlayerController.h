// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TakaPlayerController.generated.h"

class ARTSHud;
class AAICharacterBase;
class AAIControllerBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharactersControlUpdate, const TArray<AAICharacterBase*>&, OutCharacters);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetActorUpdate, const AActor*, NewActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBeforeSelectedActorUpdate, const TArray<AAICharacterBase*>&, OutControlledCharacters, const AActor*, OutActor);

UCLASS()
class ATakaPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATakaPlayerController();

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AI|Selected")
	FCharactersControlUpdate OnCharactersControlUpdated;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AI|Selected")
	FTargetActorUpdate OnTargetActorUpdated;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "AI|Selected")
	FBeforeSelectedActorUpdate OnBeforeSelectedActorUpdated;

	virtual void BeginPlay()override;
	//virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	ARTSHud * RTSHud;

	UPROPERTY(BlueprintReadOnly, Category = "AI|Selected")
	TArray<AAICharacterBase*> ControlledCharacters;

	UPROPERTY(BlueprintReadOnly, Category = "AI|Selected")
	AActor* TargetActor;

	bool bIsShiftHolding = false;

protected:

	void SelectionPressed();
	void SelectionReleased();

	void RightMouseButtonPressed();
	void RightMouseButtonReleased();

	void CharacterInteraction(AAICharacterBase * AICharacter, struct FHitResult &Hit, class AActor * InteractableActor = nullptr);

	void LeftShiftPressed();
	void LeftShiftReleased();

private:

	void StartMoveTask(AAICharacterBase * AICharacter, const FVector OutLocation, AAIControllerBase * AIControllerBase);

	class AWayPoint * SpawnWayPoint(TSubclassOf<class AWayPoint> WayPointClass, const FVector GoalLocation);

	/** Move To Interactable object */
	bool TryMove(FVector &OutLocation, AAICharacterBase * AICharacter, AAIControllerBase * AIControllerBase);

};


