// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "PlayerCameraPawn.generated.h"

/**
 * 
 */
UCLASS()
class TAKA_API APlayerCameraPawn : public ADefaultPawn
{
	GENERATED_BODY()

	APlayerCameraPawn();

	virtual void SetupPlayerInputComponent(UInputComponent * PlayerInputComponent) override;

	virtual void BeginPlay()override;
	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* CameraBoom;

	class ARTSHud * RTSHud;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (UIMin = "0.0", UIMax = "25.0"))
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (UIMin = "0.0", UIMax = "25.0"))
	float RotateSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (UIMin = "0.01", UIMax = "0.1"))
	float LookUpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (UIMin = "0.0", UIMax = "1.0"))
	float CurrentLookUpPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (UIMin = "0.0", UIMax = "100.0"))
	float ScreenEdgeZonePercent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (UIMin = "0.0", UIMax = "2000"))
	float MaxArmLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (UIMin = "0.0", UIMax = "2000"))
	float MinArmLength;
	

private:

	bool MoveForwardKeyboard = false;
	bool MoveRightKeyboard = false;


public:

	virtual void MoveForward(float Val) override;

	virtual void MoveRight(float Val) override;

	virtual void TurnAtRate(float Rate) override;

	virtual void LookUpAtRate(float Rate) override;

	virtual void MoveUp_World(float Rate) override;


	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
};
