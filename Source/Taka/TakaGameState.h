// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TakaGameState.generated.h"

USTRUCT(BlueprintType)
struct TAKA_API FTimeStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Time)
	int Minutes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Time)
	int Hours;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Time)
	int Days;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Time)
	float TimeOfDay;

	FTimeStruct() : Minutes(0), Hours(0), Days(0), TimeOfDay(0){}
	FTimeStruct(int day, int hour, int minut, float time) : Minutes(minut), Hours(hour), Days(day), TimeOfDay(time){}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameMinuteUpdate, const FTimeStruct, UpdatedTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameHourUpdate, const FTimeStruct, UpdatedTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameDayUpdate, const FTimeStruct, UpdatedTime);

UCLASS()
class TAKA_API ATakaGameState : public AGameState
{
	GENERATED_BODY()

		
protected:

	void BeginPlay();

	void Tick(float DeltaSeconds);

public:

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Time")
	FTimeStruct TimeStruct;

#pragma region Unit

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit")
	TArray<class AItem*> Items;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit")
	TArray<class AResourceBase*> Resources;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit")
	TArray<class AStructureBase*> Structures;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Unit")
	TArray<class AAICharacterBase*> AICharacters;

#pragma endregion Unit

public:

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Milestone")
	FGameMinuteUpdate OnGameMinuteUpdated;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Milestone")
	FGameHourUpdate OnGameHourUpdated;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Milestone")
	FGameDayUpdate OnGameDayUpdated;
	
};
