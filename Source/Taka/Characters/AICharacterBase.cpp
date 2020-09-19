// Copyright Epic Games, Inc. All Rights Reserved.

#include "AICharacterBase.h"
#include "../Pathfinding/WayPoint.h"
#include "Components/CapsuleComponent.h"
#include "../AI/AIControllerBase.h"
#include "../AI/TaskBase.h"
#include "../AI/BaseTasks/SWait.h"
#include "../TakaGameState.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Plugins/Runtime/ProceduralMeshComponent/Source/ProceduralMeshComponent/Public/ProceduralMeshComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AGSSCharacter

AAICharacterBase::AAICharacterBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ProceduralViewOfView = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMeshComp"));
	ProceduralViewOfView->SetupAttachment(RootComponent);
}

void AAICharacterBase::BeginPlay()
{
	Super::BeginPlay();
	ATakaGameState* const TakaGameState = GetWorld() != NULL ? GetWorld()->GetGameState<ATakaGameState>() : NULL;
	if (TakaGameState)
	{
		TakaGameState->AICharacters.AddUnique(this);
	}
}

bool AAICharacterBase::BeforeSelect_Implementation(APlayerController *PlayerController)
{
	return true;
}

bool AAICharacterBase::Select_Implementation(APlayerController *PlayerController)
{
	return true;
}

bool AAICharacterBase::UnSelect_Implementation(APlayerController *PlayerController)
{
	return true;
}

const EStrategyUnitType AAICharacterBase::GetStrategyUnitType_Implementation()
{
	return StrategyUnitType;
}

const FVector AAICharacterBase::GetCharacterLocation()
{
	FVector Location = this->GetActorLocation();
	UCapsuleComponent * Capsule = this->GetCapsuleComponent();
	float Heigth = Capsule->GetScaledCapsuleHalfHeight_WithoutHemisphere();
	Location.Z = Location.Z - Heigth;

	return Location;
}

const EAITeam AAICharacterBase::GetTeam_Implementation()
{
	return IDTeam;
}

void AAICharacterBase::GetViewPoints(TArray<FVector> &OutPoints)
{
	float CurrentAngle = SightStruct.AngleOfView / 2;
	int Rounds = CurrentAngle;

	for (int i = 0; i < Rounds; i++)
	{
		CurrentAngle = CurrentAngle - 2;

		const FVector NewDirection = UKismetMathLibrary::RotateAngleAxis(GetActorForwardVector(), CurrentAngle, FVector(0, 0, 1));

		FVector Start = GetActorLocation();
		FVector End = (NewDirection * SightStruct.RangeOfView) + GetActorLocation();

		// additional trace parameters
		FCollisionQueryParams TraceParams(FName(TEXT("InteractTrace")), false, this);
		//Re-initialize hit info
		FHitResult HitDetails = FHitResult(ForceInit);

		bool bIsHit = GetWorld()->LineTraceSingleByChannel(
			HitDetails,      // FHitResult object that will be populated with hit info
			Start,      // starting position
			End,        // end position
			ECC_GameTraceChannel3,  // collision channel - 3rd custom one
			TraceParams      // additional trace settings
		);

		if (bIsHit)
		{
			OutPoints.Add(HitDetails.Location);
		}
		else
		{
			OutPoints.Add(End);
		}
	}
}

UProceduralMeshComponent * AAICharacterBase::DrawVieldOfView()
{
	TArray<FVector> OutPoints;
	GetViewPoints(OutPoints);

	TArray<FVector> Verticles;
	TArray<int> Traingles;

	int TrainglesIndex = 1;

	Verticles.Add(FVector(0, 0, 0));

	for (int i = 0; i < OutPoints.Num() - 2; i++)
	{
		Verticles.Add(UKismetMathLibrary::InverseTransformLocation(this->GetTransform(), OutPoints[i]));
		Verticles.Add(UKismetMathLibrary::InverseTransformLocation(this->GetTransform(), OutPoints[i + 1]));
		Traingles.Add(0);
		Traingles.Add(TrainglesIndex);
		Traingles.Add(TrainglesIndex + 1);
		TrainglesIndex += 2;
	}

	//* Setup *//
	const TArray<FVector> Normals;
	const TArray<FVector2D> UV0;
	const TArray<FVector2D> UV1;
	const TArray<FVector2D> UV2;
	const TArray<FVector2D> UV3;
	const TArray<FColor> VertexColors;
	const TArray<FProcMeshTangent> Tangents;
	bool bCreateCollision = false;

	ProceduralViewOfView->CreateMeshSection(0, Verticles, Traingles, Normals, UV0, UV1, UV2, UV3, VertexColors, Tangents, bCreateCollision);

	return ProceduralViewOfView;
}

void AAICharacterBase::AddTaskToQueue(TSubclassOf<UTaskBase> NewTask, AActor * TargetActor, bool HasPriority)
{
	AAIControllerBase * AIController = Cast<AAIControllerBase>(this->GetController());
	if (CurrentActiveTask && HasPriority)
	{
		AbortTasks();
		STasks.Insert(FSTaskStruct(NewTask, TargetActor), 0);
		CurrentActiveTask->OnStartTaskCompleted.Broadcast(ETaskState::Finish);

	}
	else
	{
		STasks.Add(FSTaskStruct(NewTask, TargetActor));
	}
}

void AAICharacterBase::AbortTasks()
{
	STasks.Empty();
}
