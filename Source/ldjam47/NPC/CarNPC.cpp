// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "CarNPC.h"
/*----------------------------------------------------------------------------------------------------*/
/*override*/
void ACarNPC::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

}
/*----------------------------------------------------------------------------------------------------*/
/*override*/
void ACarNPC::MoveToTarget(AActor* target)
{
	FVector targetLocation = _startingLocation;
	targetLocation.X = targetLocation.X + _distanceBeforeTurningAround;

	FVector direction = targetLocation - _startingLocation;
	float distance = (targetLocation - GetActorLocation()).Size();

	if (distance <= _arrivingRadius)
	{
		OnArrivedToTarget(target);
		return;
	}

	AddMovementInput(direction);
}
/*----------------------------------------------------------------------------------------------------*/
void ACarNPC::OnArrivedToTarget(AActor* target)
{
	SetNPCDirection(GetOppositeDirection(GetNPCDirection()));
	_startingLocation = GetActorLocation();
	_distanceBeforeTurningAround *= (-1);
}
/*----------------------------------------------------------------------------------------------------*/
void ACarNPC::BeginPlay()
{
	Super::BeginPlay();

	_startingLocation = GetActorLocation();
	if (GetStartingNPCDirection() == EMovablePawnDirection::Left)
	{
		_distanceBeforeTurningAround *= (-1);
	}
}
/*----------------------------------------------------------------------------------------------------*/
