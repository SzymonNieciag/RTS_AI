// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSHUD.h"
#include <GameFramework/HUD.h>
#include "../Interfaces/Selectable.h"
#include "../Characters/AICharacterBase.h"
#include "TakaPlayerController.h"

void ARTSHud::DrawHUD()
{
	SelectedActors.Empty();

	if (bStartingSelecting)
	{
		CurrentPoint = GetMousePos2D();
		DrawRect(FLinearColor(1, 0, 0, 0.3f), InitialPoint.X, InitialPoint.Y, CurrentPoint.X - InitialPoint.X, CurrentPoint.Y - InitialPoint.Y);

		GetActorsInSelectionRectangle<AActor>(InitialPoint, CurrentPoint, SelectedActors, false, false);

		if (SelectedActors.Num() != 0)
		{
			SelectedFriendlyCharacters.Empty();

			for (auto* Actor : SelectedActors)
			{
				if (AAICharacterBase * FriendlyCharacter = Cast<AAICharacterBase>(Actor))
				{
					if (FriendlyCharacter->GetStrategyUnitType() == EStrategyUnitType::PlayerUnit)
					{
						SelectedFriendlyCharacters.Add(FriendlyCharacter);
						ISelectable::Execute_BeforeSelect(Actor, PlayerOwner);
					}
				}
			}
			if (SelectedFriendlyCharacters.Num() == 0)
			{
				if (GetFirstInteractableActor())
				{
					ISelectable::Execute_BeforeSelect(InteractableTargetActor, PlayerOwner);
				}
			}
		}
	}
	else
	{
		ATakaPlayerController * TakaPlayerController = Cast<ATakaPlayerController>(PlayerOwner);
		if (TakaPlayerController == nullptr)
		{
			return;
		}

		FHitResult Hit;
		TakaPlayerController->GetHitResultUnderCursor(ECC_Camera, true, Hit);

		AActor * NewActor = Hit.GetActor();
		AActor * CurrentSelectableActor = nullptr;

		ISelectable* Interface = Cast<ISelectable>(NewActor);
		if (Interface)
		{
			CurrentSelectableActor = NewActor;
			ISelectable::Execute_BeforeSelect(CurrentSelectableActor, PlayerOwner);
			TakaPlayerController->OnBeforeSelectedActorUpdated.Broadcast(TakaPlayerController->ControlledCharacters, CurrentSelectableActor);
			return;
		}
		else
		{
			TakaPlayerController->OnBeforeSelectedActorUpdated.Broadcast(TakaPlayerController->ControlledCharacters, CurrentSelectableActor);
		}
	}
}

bool ARTSHud::GetFirstInteractableActor()
{
	for (AActor * Actor : SelectedActors)
	{
		if (ISelectable * SelectableActor = Cast<ISelectable>(Actor))
		{
			InteractableTargetActor = Actor;
			return true;
		}
	}
	InteractableTargetActor = nullptr;
	return false;
}

FVector2D ARTSHud::GetMousePos2D()
{
	float PosX;
	float PosY;

	GetOwningPlayerController()->GetMousePosition(PosX, PosY);

	return FVector2D(PosX, PosY);
}

void ARTSHud::FilterByActorPriority(const TArray<AActor*> InActors, TArray<AActor*> OutActors)
{
	OutActors.Empty();

	EStrategyUnitType AvePriority = EStrategyUnitType::Neutral;

	for (AActor* EachActor : InActors)
	{
		if (ISelectable* Interface = Cast<ISelectable>(EachActor))
		{
			EStrategyUnitType PriorityToCheck = ISelectable::Execute_GetStrategyUnitType(EachActor);

			if (PriorityToCheck < AvePriority)
			{
				AvePriority = PriorityToCheck;
			}
		}
	}

	for (AActor* EachActor : InActors)
	{
		if (ISelectable* Interface = Cast<ISelectable>(EachActor))
		{
			if (AvePriority == ISelectable::Execute_GetStrategyUnitType(EachActor))
			{
				OutActors.Add(EachActor);
			}
		}
	}
}