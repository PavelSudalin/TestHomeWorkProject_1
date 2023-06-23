// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "PlatformInvocator.h"
#include "BasePlatform.generated.h"

UENUM()
enum class EPlatformBehavior : uint8
{
	OnDemand = 0,
	Loop
};

UCLASS()
class HOMEWORKPROJECT_1_API ABasePlatform : public AActor
{
	
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasePlatform();
	
	FTimeline PlatformTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* PlatformMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient)
	FVector StartLocation;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (MakeEditWidget))
	FVector EndLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCurveFloat* TimelineCurve;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EPlatformBehavior PlatformBehavior = EPlatformBehavior::OnDemand;

	UPROPERTY(EditInstanceOnly,BlueprintReadOnly)
	APlatformInvocator* PlatformInvocator;


	void OnPlatformInvoked();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PlatformTimelineUpdate(float Alpha) const;
};
