// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

APlayerCharacter::APlayerCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = 1;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
}

void APlayerCharacter::MoveForward(float Value)
{
	if (!FMath::IsNearlyZero(Value, 1e-6f))
	{
		FRotator YawRotator(0.0f, GetControlRotation().Yaw, 0.0f);
		FVector ForwardVector = YawRotator.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardVector, Value);
		//AddMovementInput(GetActorForwardVector(), Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (!FMath::IsNearlyZero(Value, 1e-6f))
	{
		FRotator YawRotator(0.0f, GetControlRotation().Yaw, 0.0f);
		FVector RightVector = YawRotator.RotateVector(FVector::RightVector);
		AddMovementInput(RightVector, Value);
		//AddMovementInput(GetActorRightVector(), Value);
	}
}

void APlayerCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void APlayerCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void APlayerCharacter::TurnAtRate(float Value)
{
	AddControllerYawInput(Value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float Value)
{
	AddControllerPitchInput(Value * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	SpringArmComponent->TargetOffset += FVector(0.0f, 0.0f, HalfHeightAdjust);
}

void APlayerCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
	SpringArmComponent->TargetOffset -= FVector(0.0f, 0.0f, HalfHeightAdjust);
}

bool APlayerCharacter::CanJumpInternal_Implementation() const
{
	bool bEncroached = true;
	
	if (bIsCrouched)
	{
		ACharacter* DefaultCharacter = GetClass()->GetDefaultObject<ACharacter>();
		check(GetCapsuleComponent());
		const float CurrentCrouchedHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		
		const float ComponentScale = GetCapsuleComponent()->GetShapeScale();
		const float OldUnscaledHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
		const float HalfHeightAdjust = DefaultCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() - OldUnscaledHalfHeight;
		const float ScaleHalfHeightAdjust  = HalfHeightAdjust * ComponentScale;

		const FVector PawnLocation  = GetCharacterMovement()->UpdatedComponent->GetComponentLocation();
		constexpr float SweepInflation = KINDA_SMALL_NUMBER * 10.f;

		FCollisionQueryParams CapsuleParams(SCENE_QUERY_STAT(CrouchTrace), false, this);
		FCollisionResponseParams ResponseParams;
		GetCharacterMovement()->InitCollisionParams(CapsuleParams, ResponseParams);

		// Compensate for the difference between current capsule size and standing size
		float Radius, HalfHeight;
		GetCapsuleComponent()->GetScaledCapsuleSize(Radius, HalfHeight);
		FVector CapsuleExtent(Radius, Radius, HalfHeight);
		constexpr float MinExtent = KINDA_SMALL_NUMBER * 10.f;
		CapsuleExtent.Z = FMath::Max(CapsuleExtent.Z - (-SweepInflation - ScaleHalfHeightAdjust), MinExtent);
		const FCollisionShape StandingCapsuleShape = FCollisionShape::MakeCapsule(CapsuleExtent);
		const ECollisionChannel CollisionChannel = GetCharacterMovement()->UpdatedComponent->GetCollisionObjectType();
		FVector StandingLocation = PawnLocation + FVector(0.f, 0.f, StandingCapsuleShape.GetCapsuleHalfHeight() - CurrentCrouchedHalfHeight);
		bEncroached = GetWorld()->OverlapBlockingTestByChannel(StandingLocation, FQuat::Identity, CollisionChannel, StandingCapsuleShape, CapsuleParams, ResponseParams);
		
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("bEncroached %i"), bEncroached));
		if (!bEncroached)
		{
			return true;
		}
	}
	
	return Super::CanJumpInternal_Implementation();
}

void APlayerCharacter::OnJumped_Implementation()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
}
