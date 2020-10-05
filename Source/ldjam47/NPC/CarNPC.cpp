// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "CarNPC.h"
#include "Components/CapsuleComponent.h"
#include "../Player/GentlemanPlayer.h"
#include "../Player/GentlemanPlayerController.h"
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
	
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACarNPC::OnOverlapBegin);
	}
}
/*----------------------------------------------------------------------------------------------------*/
void ACarNPC::OnOverlapBegin(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (otherActor == nullptr && otherActor == this && otherComp == nullptr)
	{
		return;
	}

	if (AGentlemanPlayer* player = Cast<AGentlemanPlayer>(otherActor))
	{
		if (AGentlemanPlayerController* playerController = Cast<AGentlemanPlayerController>(player->GetController()))
		{
			playerController->ApplyDamage(GetNPCDirection(), GetAttackDamage());
			OnNPCAttackedTarget.Broadcast(player);
		}
	}
}
/*----------------------------------------------------------------------------------------------------*/
