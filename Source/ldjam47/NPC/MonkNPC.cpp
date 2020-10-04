// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "MonkNPC.h"
#include "../GentlemansWorldSettings.h"
#include "../Levels/LevelManager.h"
/*----------------------------------------------------------------------------------------------------*/
/*override*/
void AMonkNPC::ApplyDamage(EMovablePawnDirection direction)
{
	Super::ApplyDamage(direction);

	if (AGentlemansWorldSettings* ws = Cast<AGentlemansWorldSettings>(GetWorldSettings()))
	{
		if (ALevelManager* levelManager = ws->GetLevelManager())
		{
			levelManager->Event_OnMonkHit();
		}
	}
}
/*----------------------------------------------------------------------------------------------------*/
/*override*/
void AMonkNPC::MoveToTarget(AActor* target)
{
	if (target == nullptr)
	{
		return;
	}

	TurnTowardsTarget(target);
}
/*----------------------------------------------------------------------------------------------------*/
