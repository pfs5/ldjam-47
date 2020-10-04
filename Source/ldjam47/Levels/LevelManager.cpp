// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "LevelManager.h"
#include "LevelPrototype.h"
/*----------------------------------------------------------------------------------------------------*/
ALevelManager::ALevelManager()
{

}
/*----------------------------------------------------------------------------------------------------*/
void ALevelManager::BeginPlay()
{
	Super::BeginPlay();

	SetCurrentLevel(_currentLevel);
}
/*----------------------------------------------------------------------------------------------------*/
void ALevelManager::SetCurrentLevel(const ELevelName& level)
{
	UnloadCurrentLevel();
	_currentLevelInstance = LoadLevel(level);
	_currentLevel = level;
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

		return world->SpawnActor<ALevelPrototype>(CDO->GetClass(), FVector(), FRotator());
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


}
/*----------------------------------------------------------------------------------------------------*/