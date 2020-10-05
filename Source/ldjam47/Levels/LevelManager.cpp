// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "LevelManager.h"
#include "LevelPrototype.h"
#include <GameFramework/PlayerController.h>
#include <Kismet/GameplayStatics.h>
/*----------------------------------------------------------------------------------------------------*/
ALevelManager::ALevelManager()
{

}
/*----------------------------------------------------------------------------------------------------*/
void ALevelManager::BeginPlay()
{
	Super::BeginPlay();

	// Init level locked status
	if (_debugMode)
	{
		_levelsLocked.Emplace(ELevelName::LevelZero, false);
		_levelsLocked.Emplace(ELevelName::LevelLimbo, false);
		_levelsLocked.Emplace(ELevelName::LevelLust, false);
		_levelsLocked.Emplace(ELevelName::LevelGluttony, false);
		_levelsLocked.Emplace(ELevelName::LevelGreed, false);
		_levelsLocked.Emplace(ELevelName::LevelAnger, false);
		_levelsLocked.Emplace(ELevelName::LevelHeresy, false);
		_levelsLocked.Emplace(ELevelName::LevelViolence, false);
		_levelsLocked.Emplace(ELevelName::LevelFraud, false);
		_levelsLocked.Emplace(ELevelName::LevelTreachery, false);
	}
	else
	{
		_levelsLocked.Emplace(ELevelName::LevelZero, false);
		_levelsLocked.Emplace(ELevelName::LevelLimbo, false);
		_levelsLocked.Emplace(ELevelName::LevelLust, true);
		_levelsLocked.Emplace(ELevelName::LevelGluttony, true);
		_levelsLocked.Emplace(ELevelName::LevelGreed, true);
		_levelsLocked.Emplace(ELevelName::LevelAnger, true);
		_levelsLocked.Emplace(ELevelName::LevelHeresy, true);
		_levelsLocked.Emplace(ELevelName::LevelViolence, true);
		_levelsLocked.Emplace(ELevelName::LevelFraud, true);
		_levelsLocked.Emplace(ELevelName::LevelTreachery, true);
	}

	SetCurrentLevel(_initialLevel);
}
/*----------------------------------------------------------------------------------------------------*/
void ALevelManager::SetCurrentLevel(const ELevelName& level)
{
	UnloadCurrentLevel();
	_currentLevelInstance = LoadLevel(level);
	_currentLevel = level;

	if (_currentLevelInstance != nullptr)
	{
		if (APawn* playerPawn = FindPlayerPawn())
		{
			playerPawn->SetActorLocation(_currentLevelInstance->GetPlayerStartLocation());
		}
	}
}
/*----------------------------------------------------------------------------------------------------*/
bool ALevelManager::IsLevelLocked(const ELevelName& level) const
{
	const bool* valuePtr = _levelsLocked.Find(level);
	if (valuePtr == nullptr)
	{
		return false;
	}

	return *valuePtr;
}
/*----------------------------------------------------------------------------------------------------*/
void ALevelManager::LockLevel(const ELevelName& level)
{
	_levelsLocked.FindOrAdd(level) = true;
}
/*----------------------------------------------------------------------------------------------------*/
void ALevelManager::UnlockLevel(const ELevelName& level)
{
	_levelsLocked.FindOrAdd(level) = false;
}
/*----------------------------------------------------------------------------------------------------*/
void ALevelManager::ResetLevels()
{
	for (uint8 i = 0; i < (uint8)ELevelName::Count; ++i)
	{
		LockLevel((ELevelName)i);
	}

	UnlockLevel(ELevelName::LevelZero);
	UnlockLevel(ELevelName::LevelLimbo);

	SetCurrentLevel(ELevelName::LevelZero);
}
/*----------------------------------------------------------------------------------------------------*/
void ALevelManager::Event_OnMonkHit()
{
	if (_currentLevelInstance != nullptr)
	{
		_currentLevelInstance->BP_Event_OnMonkHit();
	}

	// Reset to first level on monk hit
	SetCurrentLevel(ELevelName::LevelZero);
}
/*----------------------------------------------------------------------------------------------------*/
void ALevelManager::Event_OnNPCDied()
{
	if (_currentLevelInstance != nullptr)
	{
		_currentLevelInstance->Event_OnNPCDied();
	}
}
/*----------------------------------------------------------------------------------------------------*/
ALevelPrototype* ALevelManager::LoadLevel(const ELevelName& level)
{
	if (TSubclassOf<ALevelPrototype>* levelPtr = _levels.Find(level))
	{
		TSubclassOf<ALevelPrototype> levelPrototypeClass = *levelPtr;

		UWorld* world = GetWorld();
		if (world == nullptr)
		{
			return nullptr;
		}

		UObject* CDO = levelPrototypeClass->GetDefaultObject();
		if (CDO == nullptr)
		{
			return nullptr;
		}

		return world->SpawnActor<ALevelPrototype>(CDO->GetClass(), FVector::ZeroVector, FQuat::Identity.Rotator());
	}

	return nullptr;
}
/*----------------------------------------------------------------------------------------------------*/
void ALevelManager::UnloadCurrentLevel()
{
	if (_currentLevelInstance == nullptr)
	{
		return;
	}

	_currentLevelInstance->Destroy();
	_currentLevelInstance = nullptr;
}
/*----------------------------------------------------------------------------------------------------*/
APawn* ALevelManager::FindPlayerPawn() const
{
	APlayerController* pc = UGameplayStatics::GetPlayerController(this, 0);
	if (pc == nullptr)
	{
		return nullptr;
	}

	return pc->GetPawn();
}
/*----------------------------------------------------------------------------------------------------*/