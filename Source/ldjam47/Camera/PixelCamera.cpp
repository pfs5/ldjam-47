// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "PixelCamera.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
/*----------------------------------------------------------------------------------------------------*/
APixelCamera::APixelCamera()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	// Make the scene component the root component
	RootComponent = SceneComponent;

	// Setup camera defaults
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetProjectionMode(ECameraProjectionMode::Orthographic);
	CameraComponent->SetOrthoWidth(512.f);

	CameraComponent->SetupAttachment(SceneComponent);
}
/*----------------------------------------------------------------------------------------------------*/
void APixelCamera::SetTargetActor(const AActor* actor)
{
	_targetActor = actor;
}
/*----------------------------------------------------------------------------------------------------*/
void APixelCamera::UpdateCameraLocation(float DeltaTime)
{
	if (!_targetActor.IsValid())
	{
		return;
	}

	// Follow target
	FVector targetActorLocation = _targetActor->GetActorLocation();

	FVector cameraLocation = CameraComponent->GetComponentLocation();
	cameraLocation.X = targetActorLocation.X;
	cameraLocation.Z = targetActorLocation.Z;

	CameraComponent->SetWorldLocation(cameraLocation);
}
/*----------------------------------------------------------------------------------------------------*/
void APixelCamera::SnapCameraLocation(float DeltaTime)
{
	const float unitsPerPixel = 1.f / _pixelsPerUnit;

	FVector cameraLocation = CameraComponent->GetComponentLocation();
	cameraLocation.X = FMath::GridSnap(cameraLocation.X, unitsPerPixel);
	cameraLocation.Z = FMath::GridSnap(cameraLocation.Z, unitsPerPixel);
	CameraComponent->SetWorldLocation(cameraLocation);
}
/*----------------------------------------------------------------------------------------------------*/
void APixelCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCameraLocation(DeltaTime);
	SnapCameraLocation(DeltaTime);
}
/*----------------------------------------------------------------------------------------------------*/
void APixelCamera::BeginPlay()
{
	Super::BeginPlay();
}
/*----------------------------------------------------------------------------------------------------*/
void APixelCamera::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	Super::EndPlay(endPlayReason);
}
/*----------------------------------------------------------------------------------------------------*/