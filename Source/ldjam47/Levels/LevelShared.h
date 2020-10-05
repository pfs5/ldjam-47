// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#pragma once

#include "LevelShared.generated.h"
/*----------------------------------------------------------------------------------------------------*/
UENUM()
enum class ELevelName : uint8
{
	None,
	LevelZero,
	LevelLimbo,
	LevelLust,
	LevelGluttony,
	LevelGreed,
	LevelAnger,
	LevelHeresy,
	LevelViolence,
	LevelFraud,
	LevelTreachery,
	Count UMETA(Hidden)
};
/*----------------------------------------------------------------------------------------------------*/