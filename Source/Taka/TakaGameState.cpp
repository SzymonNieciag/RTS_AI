// Fill out your copyright notice in the Description page of Project Settings.


#include "TakaGameState.h"

void ATakaGameState::BeginPlay()
{
	Super::BeginPlay();

	TimeStruct.TimeOfDay += TimeStruct.Minutes;
	TimeStruct.TimeOfDay += TimeStruct.Hours * 60;
}

void ATakaGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TimeStruct.TimeOfDay += DeltaSeconds;

	int TimeDayInteger = TimeStruct.TimeOfDay;
	int CurrentMinute = TimeDayInteger % 60;

	if (CurrentMinute != TimeStruct.Minutes)
	{
		TimeStruct.Minutes++;
		OnGameMinuteUpdated.Broadcast(TimeStruct);

		if (TimeStruct.Minutes >= 60)
		{
			TimeStruct.Minutes = 0;
			TimeStruct.Hours++;
			OnGameHourUpdated.Broadcast(TimeStruct);

			if (TimeStruct.Hours >= 24)
			{
				TimeStruct.Hours = 0;
				TimeStruct.Minutes = 0;
				TimeStruct.TimeOfDay = 0;
				TimeStruct.Days++;
				OnGameDayUpdated.Broadcast(TimeStruct);
			}
		}
	}
}