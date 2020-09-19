// Copyright Epic Games, Inc. All Rights Reserved.

#include "TakaPlayerController.h"
#include "../Interfaces/Selectable.h"
#include "RTSHUD.h"
#include "PlayerCameraPawn.h"
#include "../Characters/AICharacterBase.h"
#include "../Pathfinding/WayPoint.h"
#include <Kismet/GameplayStatics.h>
#include "../AI/AIControllerBase.h"
#include "../AI/BaseTasks/SMoveTo.h"
#include "NavigationSystem.h"
#include "../Interfaces/Interactable.h"
#include <DrawDebugHelpers.h>

ATakaPlayerController::ATakaPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void ATakaPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	RTSHud = Cast<ARTSHud>(GetHUD());

	APlayerCameraPawn * PlayerCameraPawn = Cast<APlayerCameraPawn>(this->GetPawn());
	if (PlayerCameraPawn)
	{
		PlayerCameraPawn->RTSHud = RTSHud;
	}
}

void ATakaPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("LMB", IE_Pressed, this, &ATakaPlayerController::SelectionPressed);
	InputComponent->BindAction("LMB", IE_Released, this, &ATakaPlayerController::SelectionReleased);

	InputComponent->BindAction("RMB", IE_Pressed, this, &ATakaPlayerController::RightMouseButtonPressed);
	InputComponent->BindAction("RMB", IE_Released, this, &ATakaPlayerController::RightMouseButtonReleased);

	InputComponent->BindAction("LeftShift", IE_Pressed, this, &ATakaPlayerController::LeftShiftPressed);
	InputComponent->BindAction("LeftShift", IE_Released, this, &ATakaPlayerController::LeftShiftReleased);
}

void ATakaPlayerController::SelectionPressed()
{
	if (RTSHud != nullptr)
	{
		if (TargetActor)
		{
			if (ISelectable* SelectableInterface = Cast<ISelectable>(TargetActor))
			{
				ISelectable::Execute_UnSelect(TargetActor, this);
			}
		}

		RTSHud->bStartingSelecting = true;

		RTSHud->InitialPoint = RTSHud->GetMousePos2D();
	}
}

void ATakaPlayerController::SelectionReleased()
{
	if (RTSHud != nullptr)
	{
		if (RTSHud->SelectedFriendlyCharacters.Num() != 0)
		{
			for (AAICharacterBase * FriendCharacter : ControlledCharacters)
			{
				if (ISelectable* SelectableInterface = Cast<ISelectable>(FriendCharacter))
				{
					ISelectable::Execute_UnSelect(FriendCharacter, this);
				}
			}
			ControlledCharacters.Empty();

			for (AAICharacterBase * FriendCharacter : RTSHud->SelectedFriendlyCharacters)
			{
				ControlledCharacters.Add(FriendCharacter);
				if (ISelectable* TheInterface = Cast<ISelectable>(FriendCharacter))
				{
					ISelectable::Execute_Select(FriendCharacter, this);
				}
			}
			OnCharactersControlUpdated.Broadcast(ControlledCharacters);
		}
		else
		{
			TargetActor = RTSHud->InteractableTargetActor;
			if (ISelectable* TheInterface = Cast<ISelectable>(TargetActor))
			{
				ISelectable::Execute_Select(TargetActor, this);
				OnTargetActorUpdated.Broadcast(TargetActor);
			}
			else
			{
				OnTargetActorUpdated.Broadcast(nullptr);
			}
		}
		RTSHud->bStartingSelecting = false;
	}
}

void ATakaPlayerController::RightMouseButtonPressed()
{

}

void ATakaPlayerController::RightMouseButtonReleased()
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Camera, true, Hit);
	if (!Hit.bBlockingHit)
	{
		return;
	}
	if (IInteractable* InteractableInterface = Cast<IInteractable>(Hit.Actor))
	{
		AActor* InteractableActor = Cast<AActor>(Hit.Actor);
		if (InteractableActor)
		{
			for (auto * CurrentActor : ControlledCharacters)
			{
				AAICharacterBase * AICharacter = Cast<AAICharacterBase>(CurrentActor);
				if (AICharacter)
				{
					CharacterInteraction(AICharacter, Hit, InteractableActor);
				}
			}
		}
	}
	else
	{
		for (auto * CurrentCharacter : ControlledCharacters)
		{
			if (CurrentCharacter)
			{
				CharacterInteraction(CurrentCharacter, Hit);
			}
		}
	}

}

void ATakaPlayerController::CharacterInteraction(AAICharacterBase * AICharacter, FHitResult &Hit, class AActor * InteractableActor /*= nullptr*/)
{
	AAIControllerBase * AIControllerBase = Cast<AAIControllerBase>(AICharacter->GetController());
	if (!AIControllerBase)
	{
		return;
	}

	FVector OutLocation;

	if (InteractableActor)
	{
		FTransform DestinateTransform = IInteractable::Execute_GetInteractableTransform(InteractableActor, AICharacter);
		OutLocation = DestinateTransform.GetLocation();
		TryMove(OutLocation, AICharacter, AIControllerBase);

		/** Get Task After Finish Move */
		TSubclassOf<UTaskBase> TargetTask = IInteractable::Execute_GetInteractableTask(InteractableActor, AICharacter);
		AICharacter->AddTaskToQueue(TargetTask, InteractableActor, false);
	}
	else
	{
		OutLocation = Hit.Location;
		TryMove(OutLocation, AICharacter, AIControllerBase);

		Hit.Location.X += 100;
	}
}

void ATakaPlayerController::LeftShiftPressed()
{
	bIsShiftHolding = true;
}

void ATakaPlayerController::LeftShiftReleased()
{
	bIsShiftHolding = false;
}

void ATakaPlayerController::StartMoveTask(AAICharacterBase * AICharacter, const FVector OutLocation, AAIControllerBase * AIControllerBase)
{
	AWayPoint * WayPoint = SpawnWayPoint(AWayPoint::StaticClass(), OutLocation);

	if (!bIsShiftHolding)
	{
		AICharacter->AddTaskToQueue(USMoveTo::StaticClass(), WayPoint, true);
	}
	else 
	{
		AICharacter->AddTaskToQueue(USMoveTo::StaticClass(), WayPoint, false);
	}
}

AWayPoint * ATakaPlayerController::SpawnWayPoint(TSubclassOf<class AWayPoint> WayPointClass, const FVector GoalLocation)
{
	AWayPoint* SpawnedActor = nullptr;

	if (WayPointClass != NULL)
	{
		UWorld* const World = GetWorld();

		if (World)
		{
			FTransform Transform;
			Transform.SetLocation(GoalLocation);

			//spawn the actor
			SpawnedActor = World->SpawnActorDeferred<AWayPoint>(WayPointClass, Transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

			// initialize NewActor...
			SpawnedActor->SetOwner(this);

			UGameplayStatics::FinishSpawningActor(SpawnedActor, Transform);
		}
	}
	return SpawnedActor;
}

bool ATakaPlayerController::TryMove(FVector &OutLocation, AAICharacterBase * AICharacter, AAIControllerBase * AIControllerBase)
{
	UNavigationSystemV1::K2_ProjectPointToNavigation(this->GetWorld(), OutLocation, OutLocation, 0, 0, FVector(50, 50, 50));

	if (!OutLocation.Equals(FVector(0, 0, 0), 50.0f))
	{
		StartMoveTask(AICharacter, OutLocation, AIControllerBase);
		return true;
	}
	return false;
}
