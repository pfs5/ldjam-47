// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "GentlemanPlayer.h"
#include "Components/BoxComponent.h"
/*----------------------------------------------------------------------------------------------------*/
AGentlemanPlayer::AGentlemanPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	_caneHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CaneHitBox"));
	_caneHitBox->SetupAttachment(RootComponent);
}
/*----------------------------------------------------------------------------------------------------*/
/*override*/
void AGentlemanPlayer::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	const float unitsPerPixel = 1.f / _pixelsPerUnit;

	FVector location = GetActorLocation();
	location.X = FMath::GridSnap(location.X, unitsPerPixel);
	location.Z = FMath::GridSnap(location.Z, unitsPerPixel);
	SetActorLocation(location);
}
/*----------------------------------------------------------------------------------------------------*/