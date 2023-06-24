// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GCBaseCharacter.generated.h"

UCLASS(Abstract, NotBlueprintable)
class HOMEWORKPROJECT_1_API AGCBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	virtual void MoveForward(float Value) {};
	virtual void MoveRight(float Value) {};
	virtual void Turn(float Value) {};
	virtual void LookUp(float Value) {};
	virtual void TurnAtRate(float Value) {};
	virtual void LookUpAtRate(float Value) {};

	virtual void ChangeCrouchState();

	virtual void StartSprint();
	virtual void StopSprint();

	virtual void Tick(float DeltaTime) override;

	FORCEINLINE bool GetIsSprinting() {return bIsSprinting;}

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Character | Controls")
	float BaseTurnRate = 45.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Character | Controls")
	float BaseLookUpRate = 45.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Character | Movement")
	float SprintSpeed = 800.0f;

	virtual bool CanSprint();
	
private:
	float DefaultMaxMovementSpeed = 0.0f;
	bool bIsSprintRequested = false;
	bool bIsSprinting = false;
};
