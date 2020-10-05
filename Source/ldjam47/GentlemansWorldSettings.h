// Fill out your copyright notice in the Description page of Project Settings.
/*----------------------------------------------------------------------------------------------------*/
#pragma once

#include "GameFramework/WorldSettings.h"
#include "GentlemansWorldSettings.generated.h"
/*----------------------------------------------------------------------------------------------------*/
class AAudioManager;
class ALevelManager;
/*----------------------------------------------------------------------------------------------------*/
/**
 * 
 */
UCLASS(config = game, notplaceable)
class LDJAM47_API AGentlemansWorldSettings : public AWorldSettings
{
	GENERATED_BODY()
	
public:
	AGentlemansWorldSettings();

	ALevelManager* GetLevelManager() const;
	AAudioManager* GetAudioManager() const;

private:
	UPROPERTY(EditAnywhere)
	ALevelManager* _levelManager;

	UPROPERTY(EditAnywhere)
	AAudioManager* _audioManager;

};
/*----------------------------------------------------------------------------------------------------*/