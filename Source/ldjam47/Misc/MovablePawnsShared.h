// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#pragma once

/*----------------------------------------------------------------------------------------------------*/
UENUM(BlueprintType)
enum class EMovablePawnState : uint8
{
	None,
	Idle,
	Walking,
	Attacking,
	Blocking
};
/*----------------------------------------------------------------------------------------------------*/
UENUM(BlueprintType)
enum class EMovablePawnDirection : uint8
{
	Right,
	Left,
	Back,
	Front
};
/*----------------------------------------------------------------------------------------------------*/
UENUM(BlueprintType)
enum class EMovementInput : uint8
{
	None,
	Up,
	Down,
	Left,
	Right
};
/*----------------------------------------------------------------------------------------------------*/