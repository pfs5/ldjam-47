// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "GentlemanPlayer.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
/*----------------------------------------------------------------------------------------------------*/
AGentlemanPlayer::AGentlemanPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	_caneAttackFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("CaneAttackFlipbook"));
	_caneAttackFlipbook->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	_caneAttackFlipbook->Stop();
	_caneAttackFlipbook->SetLooping(false);
	_caneAttackFlipbook->SetHiddenInGame(true);

	_caneHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CaneHitBox"));
	_caneHitBox->SetupAttachment(_caneAttackFlipbook);
}
/*----------------------------------------------------------------------------------------------------*/
UPaperFlipbookComponent* AGentlemanPlayer::GetCaneAttackFlipbook() const
{
	return _caneAttackFlipbook;
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