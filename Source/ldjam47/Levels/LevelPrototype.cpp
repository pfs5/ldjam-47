// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "LevelPrototype.h"

#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "PaperSpriteComponent.h"
#include "Engine/CollisionProfile.h"
/*----------------------------------------------------------------------------------------------------*/
ALevelPrototype::ALevelPrototype()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	_background = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Background"));
	_background->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	_background->SetupAttachment(RootComponent);

	_foreground = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Foreground"));
	_foreground->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	_foreground->SetupAttachment(RootComponent);

	_boxLeft = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Left"));
	_boxLeft->SetupAttachment(RootComponent);

	_boxRight = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Right"));
	_boxRight->SetupAttachment(RootComponent);

	_boxTop = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Top"));
	_boxTop->SetupAttachment(RootComponent);

	_boxBottom = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Bottom"));
	_boxBottom->SetupAttachment(RootComponent);

}
/*----------------------------------------------------------------------------------------------------*/
const FVector& ALevelPrototype::GetPlayerStartLocation() const
{
	return _playerStartLocation;
}
/*----------------------------------------------------------------------------------------------------*/