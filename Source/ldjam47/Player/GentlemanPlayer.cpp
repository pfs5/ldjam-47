// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "GentlemanPlayer.h"
#include "../Plugins/2D/Paper2D/Source/Paper2D/Classes/PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
/*----------------------------------------------------------------------------------------------------*/
AGentlemanPlayer::AGentlemanPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	_umbrellaAttackFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("UmbrellaAttackFlipbook"));
	_umbrellaAttackFlipbook->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	_umbrellaAttackFlipbook->Stop();
	_umbrellaAttackFlipbook->SetLooping(false);
	_umbrellaAttackFlipbook->SetHiddenInGame(true);

	_umbrellaAttackHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("UmbrellaHitBox"));
	_umbrellaAttackHitBox->SetupAttachment(_umbrellaAttackFlipbook);

	_umbrellaBlockFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("UmbrellaShieldFlipbook"));
	_umbrellaBlockFlipbook->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	_umbrellaBlockFlipbook->Stop();
	_umbrellaBlockFlipbook->SetLooping(false);
	_umbrellaBlockFlipbook->SetHiddenInGame(true);
}
/*----------------------------------------------------------------------------------------------------*/
UBoxComponent* AGentlemanPlayer::GetUmbrellaAttackHitBox() const
{
	return _umbrellaAttackHitBox;
}
/*----------------------------------------------------------------------------------------------------*/
UPaperFlipbookComponent* AGentlemanPlayer::GetUmbrellaAttackFlipbook() const
{
	return _umbrellaAttackFlipbook;
}
/*----------------------------------------------------------------------------------------------------*/
UPaperFlipbookComponent* AGentlemanPlayer::GetUmbrellaBlockFlipbook() const
{
	return _umbrellaBlockFlipbook;
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