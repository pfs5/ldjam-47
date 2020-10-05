// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#include "PriestNPC.h"
#include "../GentlemansWorldSettings.h"
#include "../Levels/LevelManager.h"
#include "../Player/GentlemanPlayer.h"
#include "../Player/GentlemanPlayerController.h"
/*----------------------------------------------------------------------------------------------------*/
/*override*/
void APriestNPC::MoveToTarget(AActor* target)
{
	AGentlemanPlayer* player = Cast<AGentlemanPlayer>(target);
	if (player == nullptr)
	{
		return;
	}

	AGentlemanPlayerController* playerController = Cast<AGentlemanPlayerController>(player->GetController());
	if(playerController == nullptr)
	{
		return;
	}

	TurnTowardsTarget(target);

	if (GetNPCDirection() != playerController->GetPlayerDirection())
	{
		return;
	}

	Super::MoveToTarget(target);
}
/*----------------------------------------------------------------------------------------------------*/
