// DRAGO Entertainment (c) 2019+

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interfaces/Selectable.h"
#include "../Interfaces/Interactable.h"
#include "StrategyActor.generated.h"

class AAICharacterBase;

UCLASS()
class TAKA_API AStrategyActor : public AActor, public ISelectable, public IInteractable
{
	GENERATED_BODY()
	
public:
	AStrategyActor(const FObjectInitializer& ObjectInitializer);

	// Sets default values for this actor's properties

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StrategyActor")
	UStaticMeshComponent *MainMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	float InteractTime = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StrategyActor")
	class UArrowComponent* DestinateTransform;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StrategyActor")
	int MaxUsers = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StrategyActor")
	TArray<class AAICharacterBase*> AICharacters;

public:

	UFUNCTION(BlueprintCallable, Category = "StrategyActor")
	virtual bool TakeActor(AAICharacterBase * AICharacterBase);

	UFUNCTION(BlueprintCallable, Category = "StrategyActor")
	virtual bool LeaveActor(AAICharacterBase * AICharacterBase);

public:

#pragma region Interfaces|Selectable

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Selectable")
	EStrategyUnitType StrategyUnitType;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ISelectable")
	bool BeforeSelect(APlayerController *PlayerController);
	virtual bool BeforeSelect_Implementation(APlayerController *PlayerController);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ISelectable")
	bool Select(APlayerController *PlayerController);
	virtual bool Select_Implementation(APlayerController *PlayerController);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ISelectable")
	bool UnSelect(APlayerController *PlayerController);
	virtual bool UnSelect_Implementation(APlayerController *PlayerController);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ISelectable")
	const EStrategyUnitType GetStrategyUnitType();
	virtual const EStrategyUnitType GetStrategyUnitType_Implementation();

#pragma endregion Interfaces|Selectable

#pragma region Interfaces|Interaction

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "StrategyActor")
	TArray<TSubclassOf<class UTaskBase>> Tasks;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IInteraction")
	const FTransform GetInteractableTransform(AAICharacterBase * AICharacterBase);
	virtual const FTransform GetInteractableTransform_Implementation(AAICharacterBase * AICharacterBase);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IInteraction")
	TSubclassOf<class UTaskBase> GetInteractableTask(AAICharacterBase * AICharacterBase);
	virtual TSubclassOf<class UTaskBase> GetInteractableTask_Implementation(AAICharacterBase * AICharacterBase);

#pragma endregion Interfaces|Interaction

};
