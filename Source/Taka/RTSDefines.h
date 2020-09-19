// Copyright 2018 DRAGO. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define SURFACE_GROUND										SurfaceType1
#define SURFACE_WOOD										SurfaceType2

#define ECC_TriggerVolume								ECC_GameTraceChannel1
//#define ECC_Projectile	

UENUM(BlueprintType)
enum class EStrategyUnitType :uint8
{
	PlayerUnit = 0 UMETA(DisplayName = "PlayerUnit"),
	EnemyUnit = 1 UMETA(DisplayName = "EnemyUnit"),
	Structures = 2 UMETA(DisplayName = "Structures"),
	Resources = 3 UMETA(DisplayName = "Resources"),
	Item = 4 UMETA(DisplayName = "Item"),
	Neutral = 5 UMETA(DisplayName = "Neutral"),
};

UENUM(BlueprintType)
enum class EAIState :uint8
{
	ActionPriority = 0 UMETA(DisplayName = "ActionPriority"),
	Move = 1 UMETA(DisplayName = "Move"),
	DefaultAction = 2 UMETA(DisplayName = "DefaultAction"),
};

UENUM(BlueprintType)
enum class ECombatMode :uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	Attack = 1 UMETA(DisplayName = "Attack"),
	Searching = 2 UMETA(DisplayName = "Searching"),
};

UENUM(BlueprintType)
enum class EAITeam :uint8
{
	Player = 0 UMETA(DisplayName = "Player"),
	Pirates = 1 UMETA(DisplayName = "Pirates"),
	Animals = 2 UMETA(DisplayName = "Animals"),
};