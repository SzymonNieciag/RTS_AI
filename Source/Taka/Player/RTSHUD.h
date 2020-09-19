// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RTSHud.generated.h"

class AActor;
class AAICharacterBase;

UCLASS()
class TAKA_API ARTSHud : public AHUD
{
	GENERATED_BODY()

public:

	virtual void DrawHUD()override;

	FVector2D InitialPoint;

	FVector2D CurrentPoint;

	FVector2D GetMousePos2D();

	bool bStartingSelecting = false;

	TArray<AAICharacterBase*> SelectedFriendlyCharacters;

	AActor* InteractableTargetActor;

private:

	TArray<AActor*> SelectedActors;

	bool GetFirstInteractableActor();

	void FilterByActorPriority(const TArray<AActor*> InActors, TArray<AActor*> OutActors);
};

