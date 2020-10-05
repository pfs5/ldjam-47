// Copyright Epic Games, Inc. All Rights Reserved.
/*----------------------------------------------------------------------------------------------------*/
#include "GentlemansWorldSettings.h"
/*----------------------------------------------------------------------------------------------------*/
AGentlemansWorldSettings::AGentlemansWorldSettings()
{

}
/*----------------------------------------------------------------------------------------------------*/
ALevelManager* AGentlemansWorldSettings::GetLevelManager() const
{
	return _levelManager;
}
/*----------------------------------------------------------------------------------------------------*/
AAudioManager* AGentlemansWorldSettings::GetAudioManager() const
{
	return _audioManager;
}
/*----------------------------------------------------------------------------------------------------*/