// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#pragma once

#include "CoreMinimal.h"
#include "LevelPrototype.h"
#include "LevelManager.generated.h"
/*----------------------------------------------------------------------------------------------------*/
class ALevelPrototype;
/*----------------------------------------------------------------------------------------------------*/
UENUM()
enum class ELevelName : uint8
{
	LevelZero,
	LevelLimbo,
	LevelLust,
	LevelGluttony,
	LevelGreed,
	LevelAnger,
	LevelHeresy,
	LevelViolence,
	LevelFraud,
	LevelTreachery
};
/*----------------------------------------------------------------------------------------------------*/
UCLASS()
class LDJAM47_API ALevelManager : public AActor
{
	GENERATED_BODY()

public:
	ALevelManager();

public:
	virtual void BeginPlay() override;

private:
	void SetCurrentLevel(const ELevelName& level);

	ALevelPrototype* LoadLevel(const ELevelName& level);
	void UnloadCurrentLevel();

private:
	UPROPERTY(EditDefaultsOnly)
	ELevelName _currentLevel;

	UPROPERTY(EditDefaultsOnly)
	TMap<ELevelName, TSubclassOf<ALevelPrototype>> _levels;

private:
	UPROPERTY()
	ALevelPrototype* _currentLevelInstance = nullptr;

};
/*----------------------------------------------------------------------------------------------------*/