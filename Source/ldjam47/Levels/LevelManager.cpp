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
	_levelsLocked.Emplace(ELevelName::LevelZero, false);
	_levelsLocked.Emplace(ELevelName::LevelLimbo, false);
	_levelsLocked.Emplace(ELevelName::LevelLust, true);
	_levelsLocked.Emplace(ELevelName::LevelGluttony, true);
	_levelsLocked.Emplace(ELevelName::LevelGreed, true);
	_levelsLocked.Emplace(ELevelName::LevelAnger, true);
	_levelsLocked.Emplace(ELevelName::LevelHeresy, true);
	_levelsLocked.Emplace(ELevelName::LevelViolence, true);
	_levelsLocked.Emplace(ELevelName::LevelFraud , true);
	_levelsLocked.Emplace(ELevelName::LevelTreachery , true);

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

	// TESTING

	switch (level)
	{
		case ELevelName::LevelLimbo:
			UnlockLevel(ELevelName::LevelLust);
			break;
		case ELevelName::LevelLust:
			UnlockLevel(ELevelName::LevelGluttony);
			break;
		case ELevelName::LevelGluttony:
			UnlockLevel(ELevelName::LevelGreed);
			break;
		case ELevelName::LevelGreed:
			UnlockLevel(ELevelName::LevelAnger);
			break;
		case ELevelName::LevelAnger:
			UnlockLevel(ELevelName::LevelHeresy);
			break;
		case ELevelName::LevelHeresy:
			UnlockLevel(ELevelName::LevelViolence);
			break;
		case ELevelName::LevelViolence:
			UnlockLevel(ELevelName::LevelFraud);
			break;
		case ELevelName::LevelFraud:
			UnlockLevel(ELevelName::LevelTreachery);
			break;
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