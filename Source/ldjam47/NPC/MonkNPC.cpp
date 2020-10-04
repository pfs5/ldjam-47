// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "MonkNPC.h"
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
