// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "Door.h"
#include "PaperSpriteComponent.h"
#include "../GentlemansWorldSettings.h"
#include <Components/BoxComponent.h>
#include "../Player/GentlemanPlayer.h"
#include "LevelManager.h"
/*----------------------------------------------------------------------------------------------------*/
ADoor::ADoor()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	_doorSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Door Sprite"));
	_doorSprite->SetupAttachment(RootComponent);

	_barsSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Bars Sprite"));
	_barsSprite->SetupAttachment(RootComponent);

	_boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	_boxComponent->SetupAttachment(RootComponent);
}
/*----------------------------------------------------------------------------------------------------*/
void ADoor::BeginPlay()
{
	Super::BeginPlay();

	if (_boxComponent != nullptr)
	{
		_boxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnOverlapBegin);
	}

	if (AGentlemansWorldSettings* worldSettings = Cast<AGentlemansWorldSettings>(GetWorldSettings()))
	{
		if (ALevelManager* levelManager = worldSettings->GetLevelManager())
		{
			_isLocked = levelManager->IsLevelLocked(_targetLevel);
		}
	}

	OnIsLockedChanged();

	_isActive = false;
	GetWorldTimerManager().SetTimer(_isActiveTimerHandle, this, &ADoor::Activate, _activationTime);
}
/*----------------------------------------------------------------------------------------------------*/
void ADoor::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	GetWorldTimerManager().ClearTimer(_isActiveTimerHandle);

	Super::EndPlay(endPlayReason);
}
/*----------------------------------------------------------------------------------------------------*/
void ADoor::OnOverlapBegin(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (_isLocked || !_isActive)
	{
		return;
	}

	if (!otherActor->IsA<AGentlemanPlayer>())
	{
		return;
	}

	if (UBoxComponent* box = Cast<UBoxComponent>(otherComp))
	{
		// Ignore gentleman's box. This sucks but it's easier.
		return;
	}

	Open();
}
/*----------------------------------------------------------------------------------------------------*/
void ADoor::OnIsLockedChanged()
{
	_barsSprite->SetVisibility(_isLocked);
}
/*----------------------------------------------------------------------------------------------------*/
void ADoor::Open()
{
	AGentlemansWorldSettings* worldSetings = Cast<AGentlemansWorldSettings>(GetWorldSettings());
	if (worldSetings == nullptr)
	{
		return;
	}

	ALevelManager* levelManager = worldSetings->GetLevelManager();
	if (levelManager == nullptr)
	{
		return;
	}

	levelManager->SetCurrentLevel(_targetLevel);
}
/*----------------------------------------------------------------------------------------------------*/
void ADoor::Activate()
{
	_isActive = true;
}
/*----------------------------------------------------------------------------------------------------*/