// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "Lever.h"
#include "PaperSpriteComponent.h"
#include "../GentlemansWorldSettings.h"
#include <Components/BoxComponent.h>
#include "../Player/GentlemanPlayer.h"
#include "LevelManager.h"
/*----------------------------------------------------------------------------------------------------*/
ALever::ALever()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	_spriteLeft = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite Left"));
	_spriteLeft->SetupAttachment(RootComponent);

	_spriteRight = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite Right"));
	_spriteRight->SetupAttachment(RootComponent);

	_boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	_boxComponent->SetupAttachment(RootComponent);
}
/*----------------------------------------------------------------------------------------------------*/
void ALever::BeginPlay()
{
	Super::BeginPlay();

	if (_boxComponent != nullptr)
	{
		_boxComponent->OnComponentBeginOverlap.AddDynamic(this, &ALever::OnOverlapBegin);
	}

	if (AGentlemansWorldSettings* worldSettings = Cast<AGentlemansWorldSettings>(GetWorldSettings()))
	{
		if (ALevelManager* levelManager = worldSettings->GetLevelManager())
		{
			SetLocked(levelManager->IsLevelLocked(_targetLevel));
		}
	}

	OnStateChanged();
}
/*----------------------------------------------------------------------------------------------------*/
void ALever::OnOverlapBegin(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (!_isActive)
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

	Toggle();
}
/*----------------------------------------------------------------------------------------------------*/
void ALever::OnStateChanged()
{
	_spriteRight->SetVisibility(_isRight);
	_spriteLeft->SetVisibility(!_isRight);
}
/*----------------------------------------------------------------------------------------------------*/
void ALever::Toggle()
{
	_isRight = !_isRight;

	if (AGentlemansWorldSettings* worldSettings = Cast<AGentlemansWorldSettings>(GetWorldSettings()))
	{
		if (ALevelManager* levelManager = worldSettings->GetLevelManager())
		{
			if (IsLocked()) 
			{
				levelManager->LockLevel(_targetLevel);
			}
			else
			{
				levelManager->UnlockLevel(_targetLevel);
			}
		}
	}


	OnStateChanged();
}
/*----------------------------------------------------------------------------------------------------*/
bool ALever::IsLocked() const
{
	return !_isRight;
}
/*----------------------------------------------------------------------------------------------------*/
void ALever::SetLocked(bool value)
{
	_isRight = !value;
}
/*----------------------------------------------------------------------------------------------------*/
void ALever::SetLeverActive(bool value)
{
	_isActive = value;

	SetActorHiddenInGame(!_isActive);
}
/*----------------------------------------------------------------------------------------------------*/