// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCameraPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "RTSHUD.h"

APlayerCameraPawn::APlayerCameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SetRelativeRotation(FRotator(-50, 0, 0));


	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom);

	MoveSpeed = 5;
	ScreenEdgeZonePercent = 15;
}

// Called to bind functionality to input
void APlayerCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCameraPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCameraPawn::MoveRight);

	PlayerInputComponent->BindAxis("TurnAtRate", this, &APlayerCameraPawn::TurnAtRate);

	PlayerInputComponent->BindAxis("LookUpAtRate", this, &APlayerCameraPawn::LookUpAtRate);
}

// Called when the game starts or when spawned
void APlayerCameraPawn::BeginPlay()
{
	if (MaxArmLength < MinArmLength)
	{
		MaxArmLength = MinArmLength;
	}

	Super::BeginPlay();
}

// Called every frame
void APlayerCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (RTSHud && !RTSHud->bStartingSelecting && Controller && !MoveRightKeyboard && !MoveForwardKeyboard)
	{
		APlayerController* playerController = Cast<APlayerController>(GetController());

		if (playerController != nullptr)
		{
			FVector2D mouseLocation;
			if (playerController->GetMousePosition(mouseLocation.X, mouseLocation.Y))
			{
				int32 screenWidth = 0;
				int32 screenHeight = 0;

				playerController->GetViewportSize(screenWidth, screenHeight);

				const float xZone = screenWidth * ScreenEdgeZonePercent / 100;
				const float yZone = screenHeight * ScreenEdgeZonePercent / 100;

				FVector Direction(0, 0, 0);

				if (mouseLocation.X < xZone)		
				{			
					FRotator CameraRotation = UKismetMathLibrary::MakeRotator(0, 0, CameraBoom->GetRelativeRotation().Yaw);
					this->AddActorWorldOffset(UKismetMathLibrary::GetRightVector(CameraRotation) * -1 * MoveSpeed);
				}
				else if (mouseLocation.X > screenWidth - xZone)	
				{
					FRotator CameraRotation = UKismetMathLibrary::MakeRotator(0, 0, CameraBoom->GetRelativeRotation().Yaw);
					this->AddActorWorldOffset(UKismetMathLibrary::GetRightVector(CameraRotation)  * MoveSpeed);
				}
				if (mouseLocation.Y < yZone)		
				{				
					FRotator CameraRotation = UKismetMathLibrary::MakeRotator(0, 0, CameraBoom->GetRelativeRotation().Yaw);
					this->AddActorWorldOffset(UKismetMathLibrary::GetForwardVector(CameraRotation) * MoveSpeed);
				}
				else if (mouseLocation.Y > screenHeight - yZone)		
				{
					FRotator CameraRotation = UKismetMathLibrary::MakeRotator(0, 0, CameraBoom->GetRelativeRotation().Yaw);
					this->AddActorWorldOffset(UKismetMathLibrary::GetForwardVector(CameraRotation) * -1 * MoveSpeed);
				}

				this->AddActorWorldOffset(Direction * MoveSpeed);
			}
		}
	}

}

void APlayerCameraPawn::MoveForward(float Val)
{
	if (Val != 0 && Controller && !RTSHud->bStartingSelecting)
	{
		MoveForwardKeyboard = true;
		FRotator CameraRotation = UKismetMathLibrary::MakeRotator(0, 0, CameraBoom->GetRelativeRotation().Yaw);
		this->AddActorWorldOffset(UKismetMathLibrary::GetForwardVector(CameraRotation) * Val * MoveSpeed);
	}
	else
	{
		MoveForwardKeyboard = false;
	}
}

void APlayerCameraPawn::MoveRight(float Val)
{
	if (Val != 0 && Controller && !RTSHud->bStartingSelecting)
	{
		MoveRightKeyboard = true;
		FRotator CameraRotation = UKismetMathLibrary::MakeRotator(0, 0, CameraBoom->GetRelativeRotation().Yaw);
		this->AddActorWorldOffset(UKismetMathLibrary::GetRightVector(CameraRotation) * Val * MoveSpeed);
	}
	else
	{
		MoveRightKeyboard = false;
	}
}

void APlayerCameraPawn::TurnAtRate(float Rate)
{
	if (Rate != 0 && Controller)
	{
		FRotator CameraRotation = UKismetMathLibrary::MakeRotator(0, CameraBoom->GetRelativeRotation().Pitch, CameraBoom->GetRelativeRotation().Yaw + Rate * RotateSpeed);

		CameraBoom->SetRelativeRotation(CameraRotation);
	}
}

void APlayerCameraPawn::LookUpAtRate(float Rate)
{
	if (Controller)
	{
		CurrentLookUpPosition = UKismetMathLibrary::FClamp(CurrentLookUpPosition + (Rate * LookUpSpeed), 0, 1);

		float TargetLengthArm = UKismetMathLibrary::Lerp(MinArmLength, MaxArmLength, CurrentLookUpPosition);
		CameraBoom->TargetArmLength = UKismetMathLibrary::Lerp(CameraBoom->TargetArmLength, TargetLengthArm, 0.01f);
		
		float RollAngle = UKismetMathLibrary::Lerp(330, 290, CurrentLookUpPosition);
		FRotator TargetRotator = UKismetMathLibrary::MakeRotator(0, RollAngle, CameraBoom->GetRelativeRotation().Yaw);
		FRotator CurrentRotator = UKismetMathLibrary::RLerp(CameraBoom->GetRelativeRotation(), TargetRotator, 0.01f, true);
		CameraBoom->SetRelativeRotation(CurrentRotator);
	}
}

void APlayerCameraPawn::MoveUp_World(float Rate)
{

}
